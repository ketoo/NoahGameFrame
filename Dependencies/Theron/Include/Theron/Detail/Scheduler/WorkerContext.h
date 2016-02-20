// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_SCHEDULER_WORKERCONTEXT_H
#define THERON_DETAIL_SCHEDULER_WORKERCONTEXT_H


#include <Theron/Detail/Allocators/CachingAllocator.h>
#include <Theron/Detail/Scheduler/MailboxContext.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
Per-worker thread context structure holding data used by a worker thread.
This is a wrapper used to hold a couple of different pieces of context data.
*/
class WorkerContext
{
public:

    /**
    Constructor.
    */
    inline WorkerContext()
    {
    }

    CachingAllocator<> mMessageCache;       ///< Per-thread cache of message memory blocks.
    MailboxContext mMailboxContext;         ///< Per-thread context for mailbox processing.

private:

    WorkerContext(const WorkerContext &other);
    WorkerContext &operator=(const WorkerContext &other);
};


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_SCHEDULER_WORKERCONTEXT_H
