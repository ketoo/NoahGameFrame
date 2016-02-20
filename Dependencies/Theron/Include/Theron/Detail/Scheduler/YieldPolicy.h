// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_YIELDPOLICY_H
#define THERON_DETAIL_SCHEDULER_YIELDPOLICY_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


namespace Theron
{
namespace Detail
{


/**
Yield strategy policy implementations.
*/
class YieldPolicy
{
public:

    /**
    Spin, but yield to other threads after a timeout.
    */
    static void Hybrid(const uint32_t counter);

    /**
    Spin indefinitely (or busy-wait), with no yielding to other threads.
    */
    static void Spin(const uint32_t counter);

private:

    YieldPolicy();
    YieldPolicy(const YieldPolicy &other);
    YieldPolicy &operator=(const YieldPolicy &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_SCHEDULER_YIELDPOLICY_H
