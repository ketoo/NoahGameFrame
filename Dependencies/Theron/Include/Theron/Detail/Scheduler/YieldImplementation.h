// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_YIELDIMPLEMENTATION_H
#define THERON_DETAIL_SCHEDULER_YIELDIMPLEMENTATION_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


namespace Theron
{
namespace Detail
{


/**
A 'yield' function which can be called to yield a waiting thread so that
other threads also accessing a contested resource can progress.
*/
typedef void (* YieldFunction)(const uint32_t counter);


/**
Mailbox processor thread yield strategy policy implementation.
*/
class YieldImplementation
{
public:

    inline explicit YieldImplementation() :
      mCounter(0),
      mYieldFunction(0)
    {
    }

    THERON_FORCEINLINE void SetYieldFunction(YieldFunction yieldFunction)
    {
        mYieldFunction = yieldFunction;
    }

    THERON_FORCEINLINE void Reset()
    {
        mCounter = 0;
    }

    THERON_FORCEINLINE void Execute()
    {
        THERON_ASSERT(mYieldFunction);
        (*mYieldFunction)(mCounter);
        ++mCounter;
    }

private:

    uint32_t mCounter;
    YieldFunction mYieldFunction;
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_SCHEDULER_YIELDIMPLEMENTATION_H
