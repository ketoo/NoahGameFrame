// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_SPINLOCK_H
#define THERON_DETAIL_THREADING_SPINLOCK_H


#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11

#include <Theron/Detail/Threading/Atomic.h>
#include <Theron/Detail/Threading/Utils.h>

#elif THERON_POSIX

#include <pthread.h>

#else

#error Theron requires POSIX thread support, Boost, or Windows.

#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4127)  // Conditional expression is constant.
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
A simple spinlock thread synchronization primitive.
*/
class THERON_PREALIGN(THERON_CACHELINE_ALIGNMENT) SpinLock
{
public:

    /**
    Default constructor.
    */
    THERON_FORCEINLINE SpinLock()
    {
#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11

        mValue.Store(0);

#elif THERON_POSIX

        pthread_spin_init(&mSpinLock, 0);

#endif
    }

    /**
    Destructor.
    */
    THERON_FORCEINLINE ~SpinLock()
    {
#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11
#elif THERON_POSIX

        pthread_spin_destroy(&mSpinLock);

#endif
    }

    /**
    Locks the spinlock.
    Locking the spinlock guarantees exclusive access to a protected resource associated with it.
    \note The calling thread will busy-wait and hence this method should be used with care.
    */
    THERON_FORCEINLINE void Lock()
    {
#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11

        uint32_t backoff(0);
        while (true)
        {
            uint32_t currentValue(UNLOCKED);
            if (mValue.CompareExchangeAcquire(currentValue, LOCKED))
            {
                return;
            }

            Utils::Backoff(backoff);
        }

#elif THERON_POSIX

        pthread_spin_lock(&mSpinLock);

#endif
    }

    /**
    Unlocks the spinlock.
    Unlocking the spinlock releases exclusive access previously acquired with Lock.
    */
    THERON_FORCEINLINE void Unlock()
    {
#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11

        THERON_ASSERT(mValue.Load() == LOCKED);
        mValue.Store(UNLOCKED);
    
#elif THERON_POSIX

        pthread_spin_unlock(&mSpinLock);

#endif
    }

private:

    SpinLock(const SpinLock &other);
    SpinLock &operator=(const SpinLock &other);

#if THERON_WINDOWS || THERON_BOOST || THERON_CPP11

    static const uint32_t UNLOCKED = 0;
    static const uint32_t LOCKED = 1;

    Atomic::UInt32 mValue;

#elif THERON_POSIX

    pthread_spinlock_t mSpinLock;

#endif

} THERON_POSTALIGN(THERON_CACHELINE_ALIGNMENT);


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_THREADING_SPINLOCK_H
