// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_ATOMIC_H
#define THERON_DETAIL_THREADING_ATOMIC_H


#include <Theron/Align.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


#ifdef _MSC_VER
#pragma warning(push,0)
#endif //_MSC_VER

#if THERON_WINDOWS

#include <windows.h>

#elif THERON_BOOST

#include <External/boost/atomic.hpp>

#elif THERON_CPP11

#if THERON_GCC
#if __GNUC__ >= 4
#include <atomic>
#else
#include <cstdatomic>
#endif
#else
#include <atomic>
#endif // THERON_GCC

#elif THERON_POSIX

#include <pthread.h>

#else

#error No atomic support detected.

#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4127)  // conditional expression is constant.
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{
namespace Atomic
{


/**
Atomic 32-bit unsigned integer synchronization primitive.
*/
class UInt32
{
public:

    /**
    Constructor.
    */
    inline UInt32() : mValue(0)
    {
#if THERON_WINDOWS
#elif THERON_BOOST
#elif THERON_CPP11
#elif THERON_POSIX

        pthread_spin_init(&mSpinLock, 0);

#endif
    }

    /**
    Explicit constructor that initializes the value.
    */
    inline explicit UInt32(const uint32_t initialValue) : mValue(static_cast<int32_t>(initialValue))
    {
#if THERON_WINDOWS
#elif THERON_BOOST
#elif THERON_CPP11
#elif THERON_POSIX

        pthread_spin_init(&mSpinLock, 0);

#endif
    }

    /**
    Destructor.
    */
    inline ~UInt32()
    {
#if THERON_WINDOWS
#elif THERON_BOOST
#elif THERON_CPP11
#elif THERON_POSIX

        pthread_spin_destroy(&mSpinLock);

#endif
    }

    /**
    Atomic compare-and-exchange with 'acquire' memory ordering semantics.
    */
    THERON_FORCEINLINE bool CompareExchangeAcquire(uint32_t &currentValue, const uint32_t newValue)
    {
#if THERON_WINDOWS

        const uint32_t expectedValue(currentValue);
        currentValue = InterlockedCompareExchangeAcquire(
            reinterpret_cast<volatile LONG *>(&mValue),
            static_cast<LONG>(newValue),
            static_cast<LONG>(currentValue));

        return (currentValue == expectedValue);

#elif THERON_BOOST

        return mValue.compare_exchange_weak(
            currentValue,
            newValue,
            boost::memory_order_acquire);

#elif THERON_CPP11

        return mValue.compare_exchange_weak(
            currentValue,
            newValue,
            std::memory_order_acquire);

#elif THERON_POSIX

        bool success(false);
        pthread_spin_lock(&mSpinLock);

        const uint32_t actualValue(mValue);
        if (actualValue == currentValue)
        {
            mValue = newValue;
            success = true;
        }

        currentValue = actualValue;

        pthread_spin_unlock(&mSpinLock);
        return success;

#endif
    }

    /**
    Atomic compare-and-exchange with 'release' memory ordering semantics.
    */
    THERON_FORCEINLINE bool CompareExchangeRelease(uint32_t &currentValue, const uint32_t newValue)
    {
#if THERON_WINDOWS

        const uint32_t expectedValue(currentValue);
        currentValue = InterlockedCompareExchangeRelease(
            reinterpret_cast<volatile LONG *>(&mValue),
            static_cast<LONG>(newValue),
            static_cast<LONG>(currentValue));

        return (currentValue == expectedValue);

#elif THERON_BOOST

        return mValue.compare_exchange_weak(
            currentValue,
            newValue,
            boost::memory_order_release);

#elif THERON_CPP11

        return mValue.compare_exchange_weak(
            currentValue,
            newValue,
            std::memory_order_release);

#elif THERON_POSIX

        bool success(false);
        pthread_spin_lock(&mSpinLock);

        const uint32_t actualValue(mValue);
        if (actualValue == currentValue)
        {
            mValue = newValue;
            success = true;
        }

        currentValue = actualValue;

        pthread_spin_unlock(&mSpinLock);
        return success;

#endif
    }

    /**
    Atomic increment.
    */
    THERON_FORCEINLINE void Increment()
    {
#if THERON_WINDOWS

        InterlockedIncrement(reinterpret_cast<volatile LONG *>(&mValue));

#elif THERON_BOOST

        ++mValue;

#elif THERON_CPP11

        ++mValue;

#elif THERON_POSIX

        pthread_spin_lock(&mSpinLock);
        ++mValue;
        pthread_spin_unlock(&mSpinLock);

#endif
    }

    /**
    Atomic decrement.
    */
    THERON_FORCEINLINE void Decrement()
    {
#if THERON_WINDOWS

        InterlockedDecrement(reinterpret_cast<volatile LONG *>(&mValue));

#elif THERON_BOOST

        --mValue;

#elif THERON_CPP11

        --mValue;

#elif THERON_POSIX

        pthread_spin_lock(&mSpinLock);
        --mValue;
        pthread_spin_unlock(&mSpinLock);

#endif
    }

    /**
    Atomically get the current value.
    */
    THERON_FORCEINLINE volatile uint32_t Load() const
    {
#if THERON_WINDOWS

        return static_cast<uint32_t>(mValue);

#elif THERON_BOOST

        return mValue.load();

#elif THERON_CPP11

        return mValue.load();

#elif THERON_POSIX

        return mValue;

#endif
    }

    /**
    Atomically set the current value.
    */
    THERON_FORCEINLINE void Store(const uint32_t val)
    {
#if THERON_WINDOWS

        InterlockedExchange(
            reinterpret_cast<volatile LONG *>(&mValue),
            static_cast<LONG>(val));

#elif THERON_BOOST

        mValue.store(val);

#elif THERON_CPP11

        mValue.store(val);

#elif THERON_POSIX

        pthread_spin_lock(&mSpinLock);
        mValue = val;
        pthread_spin_unlock(&mSpinLock);

#endif
    }

private:

    UInt32(const UInt32 &other);
    UInt32 &operator=(const UInt32 &other);

#if THERON_WINDOWS

    volatile int32_t mValue;

#elif THERON_BOOST

    volatile boost::atomic_uint32_t mValue;

#elif THERON_CPP11

    volatile std::atomic_uint_least32_t mValue;

#elif THERON_POSIX

    // With POSIX threads we emulate atomics using a spinlock (ie. slow but works).
    volatile uint32_t mValue;
    mutable pthread_spinlock_t mSpinLock;

#endif

};


} // namespace Atomic
} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_THREADING_ATOMIC_H
