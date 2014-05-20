// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_BLOCKINGMONITOR_H
#define THERON_DETAIL_SCHEDULER_BLOCKINGMONITOR_H


#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/YieldStrategy.h>

#include <Theron/Detail/Threading/Condition.h>
#include <Theron/Detail/Threading/Lock.h>
#include <Theron/Detail/Threading/Mutex.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
\brief Blocking monitor thread synchronization primitive based on a condition variable.
*/
class BlockingMonitor
{
public:

    struct Context
    {
    };

    class LockType
    {
    public:

        friend class BlockingMonitor;

        THERON_FORCEINLINE explicit LockType(BlockingMonitor &monitor) : mLock(monitor.mCondition.GetMutex())
        {
        }

        THERON_FORCEINLINE void Unlock()
        {
            mLock.Unlock();
        }

        THERON_FORCEINLINE void Relock()
        {
            mLock.Relock();
        }

    private:

        LockType(const LockType &other);
        LockType &operator=(const LockType &other);

        Lock mLock;
    };

    friend class LockType;

    /**
    Constructs a monitor with the given yield strategy hint.
    */
    inline explicit BlockingMonitor(const YieldStrategy yieldStrategy);

    /**
    Initializes the context structure of a worker thread.
    \note The calling thread must be a worker thread.
    */
    inline void InitializeWorkerContext(Context *const context);

    /**
    Resets the yield backoff following a successful acquire.
    \note The calling thread should not hold a lock.
    */
    inline void ResetYield(Context *const context);

    /**
    Wakes at most one waiting thread.
    \note The calling thread should hold a lock while changing the protected state but should release it before calling Pulse.
    */
    inline void Pulse();

    /**
    Wakes all waiting threads.
    \note The calling thread should hold a lock while changing the protected state but should release it before calling PulseAll.
    */
    inline void PulseAll();

    /**
    Puts the calling thread to sleep until it is woken by a pulse.
    \note The calling thread should hold a lock and should pass the lock as a parameter.
    */
    inline void Wait(Context *const context, LockType &lock);

private:

    BlockingMonitor(const BlockingMonitor &other);
    BlockingMonitor &operator=(const BlockingMonitor &other);

    mutable Condition mCondition;
};


inline BlockingMonitor::BlockingMonitor(const YieldStrategy /*yieldStrategy*/)
{
}


inline void BlockingMonitor::InitializeWorkerContext(Context *const /*context*/)
{
}


THERON_FORCEINLINE void BlockingMonitor::ResetYield(Context *const /*context*/)
{
}


THERON_FORCEINLINE void BlockingMonitor::Pulse()
{
    mCondition.Pulse();
}


THERON_FORCEINLINE void BlockingMonitor::PulseAll()
{
    mCondition.PulseAll();
}


THERON_FORCEINLINE void BlockingMonitor::Wait(Context *const /*context*/, LockType &lock)
{
    // The lock is released and reacquired atomically inside Wait().
    mCondition.Wait(lock.mLock);
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_SCHEDULER_BLOCKINGMONITOR_H
