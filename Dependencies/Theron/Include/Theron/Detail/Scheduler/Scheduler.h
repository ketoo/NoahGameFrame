// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_SCHEDULER_H
#define THERON_DETAIL_SCHEDULER_SCHEDULER_H


#include <new>

#include <Theron/Align.h>
#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>
#include <Theron/YieldStrategy.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Directory/Directory.h>
#include <Theron/Detail/Handlers/FallbackHandlerCollection.h>
#include <Theron/Detail/Mailboxes/Mailbox.h>
#include <Theron/Detail/Scheduler/IScheduler.h>
#include <Theron/Detail/Scheduler/MailboxContext.h>
#include <Theron/Detail/Scheduler/MailboxProcessor.h>
#include <Theron/Detail/Scheduler/SchedulerHints.h>
#include <Theron/Detail/Scheduler/ThreadPool.h>
#include <Theron/Detail/Scheduler/WorkerContext.h>
#include <Theron/Detail/Threading/Atomic.h>
#include <Theron/Detail/Threading/Condition.h>
#include <Theron/Detail/Threading/Mutex.h>
#include <Theron/Detail/Threading/Thread.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#pragma warning (disable:4996)  // function or variable may be unsafe
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
Mailbox scheduler.
*/
template <class QueueType>
class Scheduler : public IScheduler
{
public:

    /**
    Constructor.
    */
    inline explicit Scheduler(
        Directory<Mailbox> *const mailboxes,
        FallbackHandlerCollection *const fallbackHandlers,
        IAllocator *const messageAllocator,
        MailboxContext *const sharedMailboxContext,
        const uint32_t nodeMask,
        const uint32_t processorMask,
        const float threadPriority,
        const YieldStrategy yieldStrategy);

    /**
    Virtual destructor.
    */
    inline virtual ~Scheduler();

    /**
    Initializes a scheduler at start of day.
    */
    inline virtual void Initialize(const uint32_t threadCount);        

    /**
    Tears down the scheduler prior to destruction.
    */
    inline virtual void Release();

    /**
    Notifies the scheduler that a worker thread is about to start executing a message handler.
    */
    inline virtual void BeginHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler);

    /**
    Notifies the scheduler that a worker thread has finished executing a message handler.
    */
    inline virtual void EndHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler);

    /**
    Schedules for processing a mailbox that has received a message.
    */
    inline virtual void Schedule(MailboxContext *const mailboxContext, Mailbox *const mailbox);

    inline virtual void SetMaxThreads(const uint32_t count);
    inline virtual void SetMinThreads(const uint32_t count);
    inline virtual uint32_t GetMaxThreads() const;
    inline virtual uint32_t GetMinThreads() const;
    inline virtual uint32_t GetNumThreads() const;
    inline virtual uint32_t GetPeakThreads() const;
    inline virtual void ResetCounters();
    inline virtual uint32_t GetCounterValue(const uint32_t counter) const;

    inline virtual uint32_t GetPerThreadCounterValues(
        const uint32_t counter,
        uint32_t *const perThreadCounts,
        const uint32_t maxCounts) const;

private:

    typedef typename QueueType::ContextType QueueContext;
    typedef Detail::ThreadPool<QueueType, WorkerContext, MailboxProcessor> ThreadPool;
    typedef typename ThreadPool::ThreadContext ThreadContext;
    typedef List<ThreadContext> ContextList;

    Scheduler(const Scheduler &other);
    Scheduler &operator=(const Scheduler &other);

    /**
    Checks whether all work queues are empty.
    */
    inline bool QueuesEmpty() const;

    /**
    Static entry point function for the manager thread.
    This is a static function that calls the real entry point member function.
    */
    inline static void ManagerThreadEntryPoint(void *const context);

    /**
    Entry point member function for the manager thread.
    */
    inline void ManagerThreadProc();

    // Referenced external objects.
    Directory<Mailbox> *mMailboxes;                     ///< Pointer to external mailbox array.
    FallbackHandlerCollection *mFallbackHandlers;       ///< Pointer to external fallback message handler collection.
    IAllocator *mMessageAllocator;                      ///< Pointer to external message memory block allocator.
    MailboxContext *mSharedMailboxContext;              ///< Pointer to external mailbox context shared by all worker threads.

    // Construction parameters.
    uint32_t mNodeMask;                                 ///< NUMA node affinity mask.
    uint32_t mProcessorMask;                            ///< Processor affinity mask with each NUMA node.
    float mThreadPriority;                              ///< Relative scheduling priority of the worker threads.

    QueueContext mSharedQueueContext;                   ///< Per-framework queue context shared by all worker threads.
    QueueType mQueue;                                   ///< Instantiation of the work queue implementation.

    // Manager thread state.
    Thread mManagerThread;                              ///< Dynamically creates and destroys the worker threads.
    bool mRunning;                                      ///< Flag used to terminate the manager thread.
    Atomic::UInt32 mTargetThreadCount;                  ///< Desired number of worker threads.
    Atomic::UInt32 mPeakThreadCount;                    ///< Peak number of worker threads.
    Atomic::UInt32 mThreadCount;                        ///< Actual number of worker threads.
    ContextList mThreadContexts;                        ///< List of worker thread context objects.
    mutable Mutex mThreadContextLock;                   ///< Protects the thread context list.
};


template <class QueueType>
inline Scheduler<QueueType>::Scheduler(
    Directory<Mailbox> *const mailboxes,
    FallbackHandlerCollection *const fallbackHandlers,
    IAllocator *const messageAllocator,
    MailboxContext *const sharedMailboxContext,
    const uint32_t nodeMask,
    const uint32_t processorMask,
    const float threadPriority,
    const YieldStrategy yieldStrategy) :
  mMailboxes(mailboxes),
  mFallbackHandlers(fallbackHandlers),
  mMessageAllocator(messageAllocator),
  mSharedMailboxContext(sharedMailboxContext),
  mNodeMask(nodeMask),
  mProcessorMask(processorMask),
  mThreadPriority(threadPriority),
  mSharedQueueContext(),
  mQueue(yieldStrategy),
  mManagerThread(),
  mRunning(false),
  mTargetThreadCount(0),
  mPeakThreadCount(0),
  mThreadCount(0),
  mThreadContexts(),
  mThreadContextLock()
{
}


template <class QueueType>
inline Scheduler<QueueType>::~Scheduler()
{
}


template <class QueueType>
inline void Scheduler<QueueType>::Initialize(const uint32_t threadCount)
{
    // Set up the shared mailbox context.
    // This context is used by worker threads when a per-thread context isn't available.
    // The mailbox context holds pointers to the scheduler and associated queue context.
    // These are used to push mailboxes that still need further processing.
    mSharedMailboxContext->mMessageAllocator = mMessageAllocator;
    mSharedMailboxContext->mFallbackHandlers = mFallbackHandlers;
    mSharedMailboxContext->mScheduler = this;
    mSharedMailboxContext->mQueueContext = &mSharedQueueContext;

    mQueue.InitializeSharedContext(&mSharedQueueContext);

    // Set the initial thread count and affinity masks.
    mThreadCount.Store(0);
    mTargetThreadCount.Store(threadCount);

    // Start the manager thread.
    mRunning = true;
    mManagerThread.Start(ManagerThreadEntryPoint, this);

    // Wait for the manager thread to start all the worker threads.
    uint32_t backoff(0);
    while (mThreadCount.Load() < mTargetThreadCount.Load())
    {
        Utils::Backoff(backoff);
    }
}


template <class QueueType>
inline void Scheduler<QueueType>::Release()
{
    // Wait for the work queue to drain, to avoid memory leaks.
    uint32_t backoff(0);
    while (!QueuesEmpty())
    {
        Utils::Backoff(backoff);
    }

    // Reset the target thread count so the manager thread will kill all the threads.
    mTargetThreadCount.Store(0);

    // Wait for all the running threads to be stopped.
    backoff = 0;
    while (mThreadCount.Load() > 0)
    {
        // Pulse any threads that are waiting so they can terminate.
        mQueue.WakeAll();
        Utils::Backoff(backoff);
    }

    // Kill the manager thread and wait for it to terminate.
    mRunning = false;
    mManagerThread.Join();

    mQueue.ReleaseSharedContext(&mSharedQueueContext);
}


template <class QueueType>
inline void Scheduler<QueueType>::BeginHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler)
{
    // Store the last send count for this handler in the context so it's available to the scheduler.
    // Reset the message send count in the context and start counting sends for this handler.
    mailboxContext->mPredictedSendCount = messageHandler->GetPredictedSendCount();
    mailboxContext->mSendCount = 0;
}


template <class QueueType>
inline void Scheduler<QueueType>::EndHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler)
{
    // Update the cached message send count for this handler.
    // These counts are used to predict which of a handler's message sends will be its last.
    messageHandler->ReportSendCount(mailboxContext->mSendCount);
}


template <class QueueType>
inline void Scheduler<QueueType>::Schedule(MailboxContext *const mailboxContext, Mailbox *const mailbox)
{
    QueueContext *const queueContext(reinterpret_cast<QueueContext *>(mailboxContext->mQueueContext));
    Mailbox *const sendingMailbox(mailboxContext->mMailbox);

    // Build a hint structure to pass to the queuing policy.
    SchedulerHints hints;

    // Whether the mailbox is being scheduled because it received a message.
    // The other possibility is that it's the sending mailbox being rescheduled.
    hints.mSend = (sendingMailbox != mailbox);

    // The predicted number of messages sent by the message handler currently being invoked (if any).
    hints.mPredictedSendCount = mailboxContext->mPredictedSendCount;

    // The actual number of messages sent so far by the handler being invoked (if any).
    hints.mSendIndex = mailboxContext->mSendCount;

    // The number of messages queued in the sending mailbox, for sends.
    // The count includes the message that is currently being processed.
    hints.mMessageCount = 0;
    if (sendingMailbox)
    {
        hints.mMessageCount = sendingMailbox->Count();
    }

    mQueue.Push(queueContext, mailbox, hints);

    // We remember the number of messages each message handler sends, so we can
    // guess whether a given send will be the last next time it's executed.
    ++mailboxContext->mSendCount;
}


template <class QueueType>
inline void Scheduler<QueueType>::SetMaxThreads(const uint32_t count)
{
    if (mTargetThreadCount.Load() > count)
    {
        mTargetThreadCount.Store(count);
    }
}


template <class QueueType>
inline void Scheduler<QueueType>::SetMinThreads(const uint32_t count)
{
    if (mTargetThreadCount.Load() < count)
    {
        mTargetThreadCount.Store(count);
    }
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetMaxThreads() const
{
    return mTargetThreadCount.Load();
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetMinThreads() const
{
    return mTargetThreadCount.Load();
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetNumThreads() const
{
    return mThreadCount.Load();
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetPeakThreads() const
{
    return mPeakThreadCount.Load();
}


template <class QueueType>
inline bool Scheduler<QueueType>::QueuesEmpty() const
{
    // Check the shared queue context.
    if (!mQueue.Empty(&mSharedQueueContext))
    {
        return false;
    }

    bool empty(true);

    mThreadContextLock.Lock();
    
    // Check the worker thread queue contexts.
    typename ContextList::Iterator contexts(mThreadContexts.GetIterator());
    while (contexts.Next())
    {
        ThreadContext *const threadContext(contexts.Get());
        if (!mQueue.Empty(&threadContext->mQueueContext))
        {
            empty = false;
            break;
        }
    }

    mThreadContextLock.Unlock();

    return empty;
}


template <class QueueType>
inline void Scheduler<QueueType>::ResetCounters()
{
    // Reset the counters in the shared thread context.
    for (uint32_t counter = 0; counter < (uint32_t) MAX_COUNTERS; ++counter)
    {
        mQueue.ResetCounter(&mSharedQueueContext, counter);
    }

    mThreadContextLock.Lock();

    // Reset the counters in all worker thread contexts.
    typename ContextList::Iterator contexts(mThreadContexts.GetIterator());
    while (contexts.Next())
    {
        ThreadContext *const threadContext(contexts.Get());
        for (uint32_t counter = 0; counter < (uint32_t) MAX_COUNTERS; ++counter)
        {
            mQueue.ResetCounter(&threadContext->mQueueContext, counter);
        }
    }

    mThreadContextLock.Unlock();
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetCounterValue(const uint32_t counter) const
{
    // Read the counter value in the shared context.
    uint32_t accumulator(mQueue.GetCounterValue(&mSharedQueueContext, counter));

    mThreadContextLock.Lock();

    // Accumulate the counter values from all thread contexts.
    typename ContextList::Iterator contexts(mThreadContexts.GetIterator());
    while (contexts.Next())
    {
        ThreadContext *const threadContext(contexts.Get());

        mQueue.AccumulateCounterValue(
            &threadContext->mQueueContext,
            counter,
            accumulator);
    }

    mThreadContextLock.Unlock();

    return accumulator;
}


template <class QueueType>
inline uint32_t Scheduler<QueueType>::GetPerThreadCounterValues(
    const uint32_t counter,
    uint32_t *const perThreadCounts,
    const uint32_t maxCounts) const
{
    uint32_t itemCount(0);

    // Read the counter value in the shared context first.
    perThreadCounts[itemCount++] = mQueue.GetCounterValue(&mSharedQueueContext, counter);

    mThreadContextLock.Lock();

    // Read the per-thread counter values into the provided array, skipping non-running contexts.
    typename ContextList::Iterator contexts(mThreadContexts.GetIterator());
    while (itemCount < maxCounts && contexts.Next())
    {
        ThreadContext *const threadContext(contexts.Get());
        if (ThreadPool::IsRunning(threadContext))
        {
            perThreadCounts[itemCount++] = mQueue.GetCounterValue(&threadContext->mQueueContext, counter);
        }
    }

    mThreadContextLock.Unlock();

    return itemCount;
}


template <class QueueType>
inline void Scheduler<QueueType>::ManagerThreadEntryPoint(void *const context)
{
    // The static entry point function is passed the object pointer as context.
    Scheduler *const framework(reinterpret_cast<Scheduler *>(context));
    framework->ManagerThreadProc();
}


template <class QueueType>
inline void Scheduler<QueueType>::ManagerThreadProc()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    while (mRunning)
    {
        mThreadContextLock.Lock();

        // Re-start stopped worker threads while the thread count is too low.
        typename ContextList::Iterator contexts(mThreadContexts.GetIterator());
        while (mThreadCount.Load() < mTargetThreadCount.Load() && contexts.Next())
        {
            ThreadContext *const threadContext(contexts.Get());
            if (!ThreadPool::IsRunning(threadContext))
            {
                if (!ThreadPool::StartThread(
                    threadContext,
                    mNodeMask,
                    mProcessorMask,
                    mThreadPriority))
                {
                    break;
                }

                mThreadCount.Increment();
            }
        }

        // Create new worker threads while the thread count is still too low.
        while (mThreadCount.Load() < mTargetThreadCount.Load())
        {
            // Create a thread context structure wrapping the worker context.
            void *const contextMemory = allocator->AllocateAligned(sizeof(ThreadContext), THERON_CACHELINE_ALIGNMENT);
            THERON_ASSERT_MSG(contextMemory, "Failed to allocate worker thread context");

            ThreadContext *const threadContext = new (contextMemory) ThreadContext(&mQueue);

            // Set up the mailbox context for the worker thread.
            // The mailbox context holds pointers to the scheduler and queue context.
            // These are used to push mailboxes that still need further processing.
            threadContext->mUserContext.mMessageCache.SetAllocator(mMessageAllocator);
            threadContext->mUserContext.mMailboxContext.mMessageAllocator = &threadContext->mUserContext.mMessageCache;
            threadContext->mUserContext.mMailboxContext.mFallbackHandlers = mFallbackHandlers;
            threadContext->mUserContext.mMailboxContext.mScheduler = this;
            threadContext->mUserContext.mMailboxContext.mQueueContext = &threadContext->mQueueContext;

            // Create a worker thread with the created context.
            if (!ThreadPool::CreateThread(threadContext))
            {
                THERON_FAIL_MSG("Failed to create worker thread");
            }

            // Start the thread on the given node and processors.
            if (!ThreadPool::StartThread(
                threadContext,
                mNodeMask,
                mProcessorMask,
                mThreadPriority))
            {
                THERON_FAIL_MSG("Failed to start worker thread");
            }

            // Remember the context so we can reuse it and eventually destroy it.
            mThreadContexts.Insert(threadContext);

            // Track the peak thread count.
            mThreadCount.Increment();
            if (mThreadCount.Load() > mPeakThreadCount.Load())
            {
                mPeakThreadCount.Store(mThreadCount.Load());
            }
        }

        // Stop some running worker threads while the thread count is too high.
        contexts = mThreadContexts.GetIterator();
        while (mThreadCount.Load() > mTargetThreadCount.Load() && contexts.Next())
        {
            ThreadContext *const threadContext(contexts.Get());
            if (ThreadPool::IsRunning(threadContext))
            {
                // Mark the thread as stopped and wake all the waiting threads.
                ThreadPool::StopThread(threadContext);
                mQueue.WakeAll();

                // Wait for the stopped thread to actually terminate.
                ThreadPool::JoinThread(threadContext);

                mThreadCount.Decrement();
            }
        }

        mThreadContextLock.Unlock();

        // The manager thread spends most of its time asleep.
        Utils::SleepThread(100);
    }

    // Free all the allocated thread context objects.
    while (!mThreadContexts.Empty())
    {
        ThreadContext *const threadContext(mThreadContexts.Front());
        mThreadContexts.Remove(threadContext);

        // Wait for the thread to stop and then destroy it.
        ThreadPool::DestroyThread(threadContext);

        // Destruct and free the per-thread context.
        threadContext->~ThreadContext();
        allocator->Free(threadContext, sizeof(ThreadContext));
    }
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_SCHEDULER_SCHEDULER_H
