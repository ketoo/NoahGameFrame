// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Scheduler/YieldPolicy.h>
#include <Theron/Detail/Threading/Utils.h>


namespace Theron
{
namespace Detail
{


void YieldPolicy::Hybrid(const uint32_t counter)
{
    if (counter < 10)
    {
        Utils::YieldToHyperthread();
    }
    else if (counter < 20)
    {
        for (uint32_t i = 0; i < 50; ++i)
        {
            Utils::YieldToHyperthread();
        }
    }
    else if (counter < 22)
    {
        Utils::YieldToLocalThread();
    }
    else
    {
        Utils::YieldToAnyThread();
    }
}


void YieldPolicy::Spin(const uint32_t counter)
{
    // This 'busy-wait' implementation never yields or sleeps.
    // It does however pause to allow another thread running on the same hyperthreaded core to proceed.
    if (counter < 10)
    {
        Utils::YieldToHyperthread();
    }
    else if (counter < 20)
    {
        for (uint32_t i = 0; i < 50; ++i)
        {
            Utils::YieldToHyperthread();
        }
    }
    else if (counter < 30)
    {
        for (uint32_t i = 0; i < 100; ++i)
        {
            Utils::YieldToHyperthread();
        }
    }
    else
    {
        for (uint32_t i = 0; i < 200; ++i)
        {
            Utils::YieldToHyperthread();
        }
    }
}


} // namespace Detail
} // namespace Theron


