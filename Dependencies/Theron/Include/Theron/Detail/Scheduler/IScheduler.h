// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_ISCHEDULER_H
#define THERON_DETAIL_SCHEDULER_ISCHEDULER_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>
#include <Theron/YieldStrategy.h>

#include <Theron/Detail/Directory/Directory.h>
#include <Theron/Detail/Handlers/IMessageHandler.h>
#include <Theron/Detail/Mailboxes/Mailbox.h>


namespace Theron
{
namespace Detail
{


class FallbackHandlerCollection;
class MailboxContext;


/**
\brief Mailbox scheduler interface.

The Scheduler class itself is templated to allow the use of different queue implementations.
This interface allows instantiations of the template against different queue types to be
referenced polymorphically.
*/
class IScheduler
{
public:

    /**
    Default constructor.
    */
    IScheduler()
    {
    }

    /**
    Virtual destructor.
    */
    virtual ~IScheduler()
    {
    }

    /**
    Initializes a scheduler at start of day.
    */
    virtual void Initialize(const uint32_t threadCount) = 0;        

    /**
    Tears down the scheduler prior to destruction.
    */
    virtual void Release() = 0;

    /**
    Notifies the scheduler that a worker thread is about to start executing a message handler.
    */
    virtual void BeginHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler) = 0;

    /**
    Notifies the scheduler that a worker thread has finished executing a message handler.
    */
    virtual void EndHandler(MailboxContext *const mailboxContext, IMessageHandler *const messageHandler) = 0;

    /**
    Schedules for processing a mailbox that has received a message.
    */
    virtual void Schedule(MailboxContext *const mailboxContext, Mailbox *const mailbox) = 0;

    /**
    Sets a maximum limit on the number of worker threads enabled in the scheduler.
    */
    virtual void SetMaxThreads(const uint32_t count) = 0;

    /**
    Sets a minimum limit on the number of worker threads enabled in the scheduler.
    */
    virtual void SetMinThreads(const uint32_t count) = 0;

    /**
    Gets the current maximum limit on the number of worker threads enabled in the scheduler.
    */
    virtual uint32_t GetMaxThreads() const = 0;

    /**
    Gets the current minimum limit on the number of worker threads enabled in the scheduler.
    */
    virtual uint32_t GetMinThreads() const = 0;

    /**
    Gets the current number of worker threads enabled in the scheduler.
    */
    virtual uint32_t GetNumThreads() const = 0;

    /**
    Gets the highest number of worker threads that was ever enabled at one time in the scheduler.
    */
    virtual uint32_t GetPeakThreads() const = 0;

    /**
    Resets all the scheduler's internal event counters to zero.
    */
    virtual void ResetCounters() = 0;

    /**
    Gets the current value of a specified event counter, accumulated for all worker threads).
    */
    virtual uint32_t GetCounterValue(const uint32_t counter) const = 0;

    /**
    Gets the current value of a specified event counter, for each worker thread individually.
    */
    virtual uint32_t GetPerThreadCounterValues(
        const uint32_t counter,
        uint32_t *const perThreadCounts,
        const uint32_t maxCounts) const = 0;

private:

    IScheduler(const IScheduler &other);
    IScheduler &operator=(const IScheduler &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_SCHEDULER_ISCHEDULER_H
