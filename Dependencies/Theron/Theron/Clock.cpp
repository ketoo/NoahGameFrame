// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Threading/Clock.h>


namespace Theron
{
namespace Detail
{


Clock::Static Clock::mStatic;


Clock::Static::Static() : mTicksPerSecond(NANOSECONDS_PER_SECOND)
{
#if THERON_WINDOWS

    // The 'ticks' are cycles in the Windows implementation.
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&counter);
    mTicksPerSecond = (uint64_t) counter.QuadPart;

#endif
}


} // namespace Detail
} // namespace Theron


