// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_CONDITION_H
#define THERON_DETAIL_THREADING_CONDITION_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Threading/Lock.h>
#include <Theron/Detail/Threading/Mutex.h>


#if THERON_MSVC
#pragma warning(push,0)
#endif // THERON_MSVC

#if THERON_WINDOWS

#include <windows.h>

#elif THERON_POSIX

#include <pthread.h>

#elif THERON_BOOST

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>

#elif THERON_CPP11

#include <thread>
#include <condition_variable>

#else

#error No condition variable support detected.

#endif

#if THERON_MSVC
#pragma warning(pop)
#endif // THERON_MSVC


namespace Theron
{
namespace Detail
{


/**
Portable condition variable synchronization primitive, sometimes also called a monitor.
*/
class Condition
{
public:

    /**
    Constructor.
    */
    inline Condition()
    {
#if THERON_WINDOWS

        InitializeConditionVariable(&mCondition);

#elif THERON_POSIX

        pthread_cond_init(&mCondition, 0);

#elif THERON_BOOST
#elif THERON_CPP11
#endif
    }

    /**
    Destructor.
    \note If the condition is destroyed while threads are still waiting on it, the result is undefined.
    */
    inline ~Condition()
    {
#if THERON_WINDOWS
#elif THERON_POSIX

        pthread_cond_destroy(&mCondition);

#elif THERON_BOOST
#elif THERON_CPP11
#endif
    }

    /**
    Returns a reference to the Mutex owned by the monitor.
    */
    THERON_FORCEINLINE Mutex &GetMutex()
    {
        return mMutex;
    }

    /**
    Suspends the calling thread until it is woken by another thread calling \ref Pulse or \ref PulseAll.
    \note The calling thread must hold a lock on the mutex associated with the condition.
    The lock owned by the caller is released, and automatically regained when the thread is woken.
    */
    THERON_FORCEINLINE void Wait(Lock &lock)
    {
#if THERON_WINDOWS

        (void) lock;
        SleepConditionVariableCS(&mCondition, &lock.mMutex.mCriticalSection, INFINITE);
    
#elif THERON_POSIX

        pthread_cond_wait(&mCondition, &lock.mMutex.mMutex);

#elif THERON_BOOST

        // The lock must be locked before calling Wait().
        THERON_ASSERT(lock.mLock.owns_lock());
        mCondition.wait(lock.mLock);

#elif THERON_CPP11

        // The lock must be locked before calling Wait().
        THERON_ASSERT(lock.mLock.owns_lock());
        mCondition.wait(lock.mLock);

#endif
    }

    /**
    Wakes a single thread that is suspended after having called \ref Wait.
    */
    THERON_FORCEINLINE void Pulse()
    {
#if THERON_WINDOWS

        WakeConditionVariable(&mCondition);

#elif THERON_POSIX

        pthread_cond_signal(&mCondition);

#elif THERON_BOOST

        mCondition.notify_one();

#elif THERON_CPP11

        mCondition.notify_one();

#endif
    }

    /**
    Wakes all threads that are suspended after having called \ref Wait.
    */
    THERON_FORCEINLINE void PulseAll()
    {
#if THERON_WINDOWS

         WakeAllConditionVariable(&mCondition);

#elif THERON_POSIX

        pthread_cond_broadcast(&mCondition);

#elif THERON_BOOST

        mCondition.notify_all();

#elif THERON_CPP11

        mCondition.notify_all();

#endif
    }

private:

    Condition(const Condition &other);
    Condition &operator=(const Condition &other);

    Mutex mMutex;

#if THERON_WINDOWS

    CONDITION_VARIABLE mCondition;

#elif THERON_POSIX

    pthread_cond_t mCondition;

#elif THERON_BOOST

    boost::condition_variable mCondition;

#elif THERON_CPP11

    std::condition_variable mCondition;

#endif

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_THREADING_CONDITION_H
