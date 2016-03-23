// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_MUTEX_H
#define THERON_DETAIL_THREADING_MUTEX_H


#include <Theron/Defines.h>


#if THERON_MSVC
#pragma warning(push,0)
#endif // THERON_MSVC

#if THERON_WINDOWS

#include <windows.h>

#elif THERON_POSIX

#include <pthread.h>

#elif THERON_BOOST

#include <boost/thread/mutex.hpp>

#elif THERON_CPP11

#include <thread>
#include <mutex>

#else

#error No mutex support detected.

#endif

#if THERON_MSVC
#pragma warning(pop)
#endif // THERON_MSVC


namespace Theron
{
namespace Detail
{


/**
Portable mutex synchronization primitive.
*/
class Mutex
{
public:

    friend class Condition;
    friend class Lock;

    /**
    Default constructor.
    */
    THERON_FORCEINLINE Mutex()
    {
#if THERON_WINDOWS

        InitializeCriticalSection(&mCriticalSection);

#elif THERON_POSIX

        pthread_mutex_init(&mMutex, 0);

#elif THERON_BOOST
#elif THERON_CPP11
#endif
    }

    /**
    Destructor.
    */
    THERON_FORCEINLINE ~Mutex()
    {
#if THERON_WINDOWS

        DeleteCriticalSection(&mCriticalSection);

#elif THERON_POSIX

        pthread_mutex_destroy(&mMutex);

#elif THERON_BOOST
#elif THERON_CPP11
#endif
    }

    /**
    Locks the mutex, guaranteeing exclusive access to a protected resource associated with it.
    \note This is a blocking call and should be used with care to avoid deadlocks.
    */
    THERON_FORCEINLINE void Lock()
    {
#if THERON_WINDOWS

        EnterCriticalSection(&mCriticalSection);

#elif THERON_POSIX

        pthread_mutex_lock(&mMutex);

#elif THERON_BOOST

        mMutex.lock();

#elif THERON_CPP11

        mMutex.lock();

#endif
    }

    /**
    Unlocks the mutex, releasing exclusive access to a protected resource associated with it.
    */
    THERON_FORCEINLINE void Unlock()
    {
#if THERON_WINDOWS

        LeaveCriticalSection(&mCriticalSection);

#elif THERON_POSIX

        pthread_mutex_unlock(&mMutex);

#elif THERON_BOOST

        mMutex.unlock();

#elif THERON_CPP11

        mMutex.unlock();

#endif
    }

private:

    Mutex(const Mutex &other);
    Mutex &operator=(const Mutex &other);

#if THERON_WINDOWS

    CRITICAL_SECTION mCriticalSection;

#elif THERON_POSIX

    pthread_mutex_t mMutex;

#elif THERON_BOOST

    boost::mutex mMutex;

#elif THERON_CPP11

    std::mutex mMutex;

#endif

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_THREADING_MUTEX_H
