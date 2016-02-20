// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Detail/Handlers/DefaultHandlerCollection.h>


namespace Theron
{
namespace Detail
{


DefaultHandlerCollection::DefaultHandlerCollection() :
  mHandler(0),
  mNewHandler(0),
  mHandlersDirty(false)
{
}


DefaultHandlerCollection::~DefaultHandlerCollection()
{
    Clear();
}


bool DefaultHandlerCollection::Clear()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    // Destroy any currently set handlers.
    if (mHandler)
    {
        mHandler->~IDefaultHandler();
        allocator->Free(mHandler);
        mHandler = 0;
    }

    if (mNewHandler)
    {
        mNewHandler->~IDefaultHandler();
        allocator->Free(mNewHandler);
        mNewHandler = 0;
    }

    mHandlersDirty = false;
    return true;
}


bool DefaultHandlerCollection::Handle(Actor *const actor, const IMessage *const message)
{
    bool handled(false);

    THERON_ASSERT(actor);
    THERON_ASSERT(message);

    // Update the handler list if there have been changes.
    if (mHandlersDirty)
    {
        UpdateHandlers();
    }

    if (mHandler)
    {
        mHandler->Handle(actor, message);
        handled = true;
    }

    return handled;
}


void DefaultHandlerCollection::UpdateHandlers()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = false;

    // Destroy any currently set handler.
    if (mHandler)
    {
        mHandler->~IDefaultHandler();
        allocator->Free(mHandler);
        mHandler = 0;
    }

    // Make the new handler (if any) the current handler.
    mHandler = mNewHandler;
    mNewHandler = 0;
}


} // namespace Detail
} // namespace Theron


