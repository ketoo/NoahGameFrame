// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_THREADPOOL_H
#define THERON_DETAIL_SCHEDULER_THREADPOOL_H


#include <new>

#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Threading/Thread.h>
#include <Theron/Detail/Threading/Utils.h>


namespace Theron
{
namespace Detail
{


/**
A pool of worker threads that process a queue of work mailboxes.
*/
template <class QueueType, class ContextType, class ProcessorType>
class ThreadPool
{
public:

    typedef typename QueueType::ItemType ItemType;
    typedef typename QueueType::ContextType QueueContext;

    /**
    User-allocated per-thread context structure.
    The client must allocate one of these for each thread it creates.
    The context structure derives from List<ThreadContext>::Node so can be stored in lists.
    */
    class ThreadContext : public List<ThreadContext>::Node
    {
    public:

        /**
        Constructor. Creates a ThreadContext wrapping a pointer to a per-thread scheduler object.
        */
        inline explicit ThreadContext(QueueType *const queue) :
          mNodeMask(0),
          mProcessorMask(0),
          mThreadPriority(0.0f),
          mStarted(false),
          mThread(0),
          mQueue(queue),
          mQueueContext(),
          mUserContext()
        {
        }

        // Internal
        uint32_t mNodeMask;                     ///< Bit-field NUMA node affinity mask for the created thread.
        uint32_t mProcessorMask;                ///< Bit-field processor affinity mask within specified nodes.
        float mThreadPriority;                  ///< Relative scheduling priority of the thread.
        bool mStarted;                          ///< Indicates whether the thread has started.
        Thread *mThread;                        ///< Pointer to the thread object.

        // Client
        QueueType *const mQueue;                ///< Pointer to the queue serviced by the worker threads.
        QueueContext mQueueContext;             ///< Queue context associated with the worker thread.
        ContextType mUserContext;               ///< User context data associated with the worker thread.

    private:

        ThreadContext(const ThreadContext &other);
        ThreadContext &operator=(const ThreadContext &other);
    };

    /**
    Creates an additional worker thread for processing of work items.
    The thread is created but not yet started - call StartThread for that.
    \param threadContext Pointer to a caller-allocated context object for the thread.
    */
    inline static bool CreateThread(ThreadContext *const threadContext);

    /**
    Starts the given thread, which must have been created with CreateThread.
    \param workQueue Pointer to the shared work queue that the thread will service.
    \param nodeMask Bit-mask specifying on which NUMA processor nodes the thread may execute.
    \param processorMask Bit-mask specifying a subset of the processors in each indicated NUMA processor node.
    \param threadPriority Relative scheduling priority of the thread.
    */
    inline static bool StartThread(
        ThreadContext *const threadContext,
        const uint32_t nodeMask,
        const uint32_t processorMask,
        const float threadPriority);

    /**
    Stops the given thread, which must have been started with StartThread.
    */
    inline static bool StopThread(ThreadContext *const threadContext);

    /**
    Waits for the given thread, which must have been stopped with StopThread, to terminate.
    */
    inline static bool JoinThread(ThreadContext *const threadContext);

    /**
    Destroys the given thread, which must have been stopped with StopThread and JoinThread.
    */
    inline static bool DestroyThread(ThreadContext *const threadContext);

    /**
    Returns true if the given thread has been started but not stopped.
    */
    inline static bool IsRunning(ThreadContext *const threadContext);

    /**
    Returns true if the given thread has started.
    */
    inline static bool IsStarted(ThreadContext *const threadContext);

private:

    ThreadPool(const ThreadPool &other);
    ThreadPool &operator=(const ThreadPool &other);

    /**
    Worker thread entry point function.
    Only global (static) functions can be used as thread entry points.
    \param context Pointer to a context object that provides the context in which the thread is run.
    */
    inline static void ThreadEntryPoint(void *const context);
};


template <class QueueType, class ContextType, class ProcessorType>
inline bool ThreadPool<QueueType, ContextType, ProcessorType>::CreateThread(ThreadContext *const threadContext)
{
    // Allocate a new thread, aligning the memory to a cache-line boundary to reduce false-sharing of cache-lines.
    void *const threadMemory = AllocatorManager::GetCache()->AllocateAligned(sizeof(Thread), THERON_CACHELINE_ALIGNMENT);
    if (threadMemory == 0)
    {
        return false;
    }

    // Construct the thread object.
    Thread *const thread = new (threadMemory) Thread();

    // Set up the private part of the user-allocated context. We pass in a pointer to the
    // threadpool instance so we can use a member function as the entry point.
    threadContext->mStarted = false;
    threadContext->mThread = thread;
   
    return true;
}


template <class QueueType, class ContextType, class ProcessorType>
inline bool ThreadPool<QueueType, ContextType, ProcessorType>::StartThread(
    ThreadContext *const threadContext,
    const uint32_t nodeMask,
    const uint32_t processorMask,
    const float threadPriority)
{
    THERON_ASSERT(threadContext->mThread);
    THERON_ASSERT(threadContext->mThread->Running() == false);

    threadContext->mNodeMask = nodeMask;
    threadContext->mProcessorMask = processorMask;
    threadContext->mThreadPriority = threadPriority;

    // Register the worker's queue context with the queue.
    threadContext->mQueue->InitializeWorkerContext(&threadContext->mQueueContext);

    // Start the thread, running it via a static (non-member function) entry point that wraps the real member function.
    threadContext->mThread->Start(ThreadEntryPoint, threadContext);

    return true;
}


template <class QueueType, class ContextType, class ProcessorType>
inline bool ThreadPool<QueueType, ContextType, ProcessorType>::StopThread(ThreadContext *const threadContext)
{
    THERON_ASSERT(threadContext->mThread);
    THERON_ASSERT(threadContext->mThread->Running());

    // Deregister the worker's queue context.
    // Typically this instructs the thread to terminate.
    threadContext->mQueue->ReleaseWorkerContext(&threadContext->mQueueContext);

    return true;
}


template <class QueueType, class ContextType, class ProcessorType>
inline bool ThreadPool<QueueType, ContextType, ProcessorType>::JoinThread(ThreadContext *const threadContext)
{
    THERON_ASSERT(threadContext->mThread);
    THERON_ASSERT(threadContext->mThread->Running());

    // Wait for the thread to finish.
    threadContext->mThread->Join();

    return true;
}


template <class QueueType, class ContextType, class ProcessorType>
inline bool ThreadPool<QueueType, ContextType, ProcessorType>::DestroyThread(ThreadContext *const threadContext)
{
    THERON_ASSERT(threadContext->mThread);
    THERON_ASSERT(threadContext->mThread->Running() == false);

    // Destruct the thread object explicitly since we constructed using placement new.
    threadContext->mThread->~Thread();

    // Free the memory for the thread.
    AllocatorManager::GetCache()->Free(threadContext->mThread, sizeof(Thread));
    threadContext->mThread = 0;

    return true;
}


template <class QueueType, class ContextType, class ProcessorType>
THERON_FORCEINLINE bool ThreadPool<QueueType, ContextType, ProcessorType>::IsRunning(ThreadContext *const threadContext)
{
    return threadContext->mQueue->Running(&threadContext->mQueueContext);
}


template <class QueueType, class ContextType, class ProcessorType>
THERON_FORCEINLINE bool ThreadPool<QueueType, ContextType, ProcessorType>::IsStarted(ThreadContext *const threadContext)
{
    return threadContext->mStarted;
}


template <class QueueType, class ContextType, class ProcessorType>
inline void ThreadPool<QueueType, ContextType, ProcessorType>::ThreadEntryPoint(void *const context)
{
    // The static entry point function is provided with a pointer to a context structure.
    // The context structure is specific to this worker thread.
    ThreadContext *const threadContext(reinterpret_cast<ThreadContext *>(context));
    QueueType *const queue(threadContext->mQueue);
    QueueContext *const queueContext(&threadContext->mQueueContext);
    ContextType *const userContext(&threadContext->mUserContext);

    // Set the thread's scheduling priority, NUMA node affinity and processor affinity.
    Utils::SetThreadAffinity(threadContext->mNodeMask, threadContext->mProcessorMask);
    Utils::SetThreadRelativePriority(threadContext->mThreadPriority);

    // Mark the thread as started so the caller knows they can start issuing work.
    threadContext->mStarted = true;

    // Process items until told to stop.
    while (queue->Running(queueContext))
    {
        if (ItemType *const item = queue->Pop(queueContext))
        {
            ProcessorType::Process(userContext, item);
        }
    }
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_SCHEDULER_THREADPOOL_H
