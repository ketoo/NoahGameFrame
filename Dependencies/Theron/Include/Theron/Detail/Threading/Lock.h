// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_LOCK_H
#define THERON_DETAIL_THREADING_LOCK_H


#include <Theron/Defines.h>

#include <Theron/Detail/Threading/Mutex.h>

#if THERON_MSVC
#pragma warning(push,0)
#endif // THERON_MSVC

#if THERON_WINDOWS
#elif THERON_POSIX

#include <pthread.h>

#elif THERON_BOOST

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#elif THERON_CPP11

#include <thread>

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
Portable lock synchronization primitive.
This class is a helper which allows a mutex to be locked and automatically locked within a scope.
*/
class Lock
{
public:

    friend class Condition;

    /**
    Constructor. Locks the given mutex object.
    */
    THERON_FORCEINLINE explicit Lock(Mutex &mutex) :
#if THERON_WINDOWS
      mMutex(mutex)
#elif THERON_POSIX
      mMutex(mutex)
#elif THERON_BOOST
      mLock(mutex.mMutex)
#elif THERON_CPP11
    mLock(mutex.mMutex)
#endif
    {
#if THERON_WINDOWS

        mMutex.Lock();

#elif THERON_POSIX

        mMutex.Lock();

#elif THERON_BOOST

        // The wrapped boost::unique_lock locks the mutex itself.

#elif THERON_CPP11

        // The wrapped std::unique_lock locks the mutex itself.

#endif
    }

    /**
    Destructor. Automatically unlocks the locked mutex that was locked on construction.
    */
    THERON_FORCEINLINE ~Lock()
    {
#if THERON_WINDOWS

        mMutex.Unlock();

#elif THERON_POSIX

        mMutex.Unlock();

#elif THERON_BOOST

        // The wrapped boost::unique_lock unlocks the mutex itself.

#elif THERON_CPP11

        // The wrapped std::unique_lock unlocks the mutex itself.

#endif
    }

    /**
    Explicitly and temporarily unlocks the locked mutex.
    \note The caller should always call \ref Relock after this call and before destruction.
    */
    THERON_FORCEINLINE void Unlock()
    {
#if THERON_WINDOWS

        mMutex.Unlock();

#elif THERON_POSIX

        mMutex.Unlock();

#elif THERON_BOOST

        THERON_ASSERT(mLock.owns_lock() == true);
        mLock.unlock();

#elif THERON_CPP11

        THERON_ASSERT(mLock.owns_lock() == true);
        mLock.unlock();

#endif
    }

    /**
    Re-locks the associated mutex, which must have been previously unlocked with \ref Unlock.
    \note Relock can only be called after a preceding call to \ref Unlock.
    */
    THERON_FORCEINLINE void Relock()
    {
#if THERON_WINDOWS

        mMutex.Lock();

#elif THERON_POSIX

        mMutex.Lock();

#elif THERON_BOOST

        THERON_ASSERT(mLock.owns_lock() == false);
        mLock.lock();

#elif THERON_CPP11

        THERON_ASSERT(mLock.owns_lock() == false);
        mLock.lock();

#endif
    }

private:

    Lock(const Lock &other);
    Lock &operator=(const Lock &other);

#if THERON_WINDOWS

    Mutex &mMutex;

#elif THERON_POSIX

    Mutex &mMutex;

#elif THERON_BOOST

    boost::unique_lock<boost::mutex> mLock;

#elif THERON_CPP11

    std::unique_lock<std::mutex> mLock;

#endif

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_THREADING_LOCK_H
