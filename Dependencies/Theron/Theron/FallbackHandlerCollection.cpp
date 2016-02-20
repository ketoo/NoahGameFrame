// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Detail/Handlers/FallbackHandlerCollection.h>


namespace Theron
{
namespace Detail
{


FallbackHandlerCollection::FallbackHandlerCollection() :
  mHandler(0),
  mNewHandler(0),
  mHandlersDirty(false)
{
}


FallbackHandlerCollection::~FallbackHandlerCollection()
{
    Clear();
}


bool FallbackHandlerCollection::Clear()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    // Destroy any currently set handlers.
    if (mHandler)
    {
        mHandler->~IFallbackHandler();
        allocator->Free(mHandler);
        mHandler = 0;
    }

    if (mNewHandler)
    {
        mNewHandler->~IFallbackHandler();
        allocator->Free(mNewHandler);
        mNewHandler = 0;
    }

    mHandlersDirty = false;
    return true;
}


bool FallbackHandlerCollection::Handle(const IMessage *const message)
{
    bool handled(false);

    THERON_ASSERT(message);

    // Update the handler list if there have been changes.
    if (mHandlersDirty)
    {
        UpdateHandlers();
    }

    if (mHandler)
    {
        mHandler->Handle(message);
        handled = true;
    }

    return handled;
}


void FallbackHandlerCollection::UpdateHandlers()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = false;

    // Destroy any currently set handler.
    if (mHandler)
    {
        mHandler->~IFallbackHandler();
        allocator->Free(mHandler);
        mHandler = 0;
    }

    // Make the new handler (if any) the current handler.
    mHandler = mNewHandler;
    mNewHandler = 0;
}


} // namespace Detail
} // namespace Theron


