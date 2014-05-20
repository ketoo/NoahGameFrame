// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/AllocatorManager.h>


namespace Theron
{


DefaultAllocator AllocatorManager::smDefaultAllocator;
AllocatorManager::CacheType AllocatorManager::smCache(&smDefaultAllocator);


void AllocatorManager::SetAllocator(IAllocator *const allocator)
{
    // This method should only be called once, at start of day.
    THERON_ASSERT_MSG(smDefaultAllocator.GetBytesAllocated() == 0, "SetAllocator can't be called while Theron objects are alive");

    // We don't bother to make this thread-safe because it should only be called at start-of-day.
    if (allocator)
    {
        smCache.SetAllocator(allocator);
    }
    else
    {
        smCache.SetAllocator(&smDefaultAllocator);
    }
}


} // namespace Theron


