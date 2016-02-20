// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_MAILBOXCONTEXT_H
#define THERON_DETAIL_SCHEDULER_MAILBOXCONTEXT_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Handlers/FallbackHandlerCollection.h>
#include <Theron/Detail/Mailboxes/Mailbox.h>
#include <Theron/Detail/Scheduler/IScheduler.h>


namespace Theron
{
namespace Detail
{


/**
Context structure holding data used by a worker thread to process mailboxes.

\note The members of a single context are all accessed only by one worker thread
so we don't need to worry about shared writes, including false sharing.
*/
class MailboxContext
{
public:

    /**
    Constructor.
    */
    inline MailboxContext() :
      mScheduler(0),
      mQueueContext(0),
      mFallbackHandlers(0),
      mMessageAllocator(0),
      mMailbox(0),
      mPredictedSendCount(0),
      mSendCount(0)
    {
    }

    IScheduler *mScheduler;                             ///< Pointer to the associated scheduler.
    void *mQueueContext;                                ///< Pointer to the associated queue context.
    FallbackHandlerCollection *mFallbackHandlers;       ///< Pointer to fallback handlers for undelivered messages.
    IAllocator *mMessageAllocator;                      ///< Pointer to message memory block allocator.
    Mailbox *mMailbox;                                  ///< Pointer to the mailbox that is being processed.
    uint32_t mPredictedSendCount;                       ///< Number of messages predicted to be sent by the handler.
    uint32_t mSendCount;                                ///< Messages sent so far by the handler being executed.

private:

    MailboxContext(const MailboxContext &other);
    MailboxContext &operator=(const MailboxContext &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_SCHEDULER_MAILBOXCONTEXT_H
