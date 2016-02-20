// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_THREADING_CLOCK_H
#define THERON_DETAIL_THREADING_CLOCK_H


#include <Theron/Defines.h>
#include <Theron/BasicTypes.h>


#if THERON_MSVC
#pragma warning(push,0)
#endif // THERON_MSVC

#if THERON_WINDOWS
#include <windows.h>
#elif THERON_POSIX
#include <time.h>
#include <sys/time.h>
#endif

#if THERON_MSVC
#pragma warning(pop)
#endif // THERON_MSVC


namespace Theron
{
namespace Detail
{


/**
Static helper class that queries system performance timers.
*/
class Clock
{
public:

    /**
    Queries the clock for a timestamp in ticks.
    The time-length of a tick is implementation-dependent.
    */
    THERON_FORCEINLINE static uint64_t GetTicks()
    {
#if THERON_WINDOWS

        // The 'ticks' are cycles in the Windows implementation.
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return (uint64_t) counter.QuadPart;

#elif THERON_POSIX

        // The 'ticks' are nanoseconds in the POSIX implementation.
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t) ts.tv_sec * NANOSECONDS_PER_SECOND + (uint64_t) ts.tv_nsec;

#else

        return 0;

#endif
    }

    /**
    Queries the clock for its resolution in ticks-per-second.
    */
    THERON_FORCEINLINE static uint64_t GetFrequency()
    {
        return mStatic.mTicksPerSecond;
    }

private:

    struct Static
    {
        Static();

        uint64_t mTicksPerSecond;
    };

    static const uint64_t NANOSECONDS_PER_SECOND = 1000000000ULL;
    static Static mStatic;
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_THREADING_CLOCK_H
