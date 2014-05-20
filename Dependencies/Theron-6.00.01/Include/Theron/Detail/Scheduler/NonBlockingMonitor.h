// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_NONBLOCKINGMONITOR_H
#define THERON_DETAIL_SCHEDULER_NONBLOCKINGMONITOR_H


#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/YieldStrategy.h>

#include <Theron/Detail/Scheduler/YieldImplementation.h>
#include <Theron/Detail/Scheduler/YieldPolicy.h>
#include <Theron/Detail/Threading/SpinLock.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
\brief Non-blocking monitor thread synchronization primitive based on a spinlock.
*/
class NonBlockingMonitor
{
public:

    struct Context
    {
        YieldImplementation mYield;
    };

    class LockType
    {
    public:

        THERON_FORCEINLINE explicit LockType(NonBlockingMonitor &monitor) : mSpinLock(monitor.mSpinLock)
        {
            mSpinLock.Lock();
        }

        THERON_FORCEINLINE ~LockType()
        {
            mSpinLock.Unlock();
        }

        THERON_FORCEINLINE void Unlock()
        {
            mSpinLock.Unlock();
        }

        THERON_FORCEINLINE void Relock()
        {
            mSpinLock.Lock();
        }

    private:

        LockType(const LockType &other);
        LockType &operator=(const LockType &other);

        SpinLock &mSpinLock;
    };

    friend class LockType;

    /**
    Constructs a monitor with the given yield strategy hint.
    */
    inline explicit NonBlockingMonitor(const YieldStrategy yieldStrategy);

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

    NonBlockingMonitor(const NonBlockingMonitor &other);
    NonBlockingMonitor &operator=(const NonBlockingMonitor &other);

    YieldStrategy mYieldStrategy;
    mutable SpinLock mSpinLock;
};


inline NonBlockingMonitor::NonBlockingMonitor(const YieldStrategy yieldStrategy) : mYieldStrategy(yieldStrategy)
{
}


inline void NonBlockingMonitor::InitializeWorkerContext(Context *const context)
{
    switch (mYieldStrategy)
    {
        default:                        context->mYield.SetYieldFunction(&Detail::YieldPolicy::Hybrid);     break;
        case YIELD_STRATEGY_HYBRID:     context->mYield.SetYieldFunction(&Detail::YieldPolicy::Hybrid);     break;
        case YIELD_STRATEGY_SPIN:       context->mYield.SetYieldFunction(&Detail::YieldPolicy::Spin);       break;
    }
}


THERON_FORCEINLINE void NonBlockingMonitor::ResetYield(Context *const context)
{
    context->mYield.Reset();
}


THERON_FORCEINLINE void NonBlockingMonitor::Pulse()
{
}


THERON_FORCEINLINE void NonBlockingMonitor::PulseAll()
{
}


THERON_FORCEINLINE void NonBlockingMonitor::Wait(Context *const context, LockType &lock)
{
    lock.Unlock();
    context->mYield.Execute();
    lock.Relock();
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_SCHEDULER_NONBLOCKINGMONITOR_H
