// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_DIRECTORY_DIRECTORY_H
#define THERON_DETAIL_DIRECTORY_DIRECTORY_H


#include <new>

#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Threading/Mutex.h>


namespace Theron
{
namespace Detail
{


/**
A registry that maps unique indices to addressable entities.
*/
template <class EntryType>
class Directory
{
public:

    /**
    Default constructor.
    */
    Directory();

    /**
    Destructor.
    */
    ~Directory();

    /**
    Finds and claims a free index for an entity.
    */
    uint32_t Allocate(uint32_t index = 0);

    /**
    Gets a reference to the entry with the given index.
    The entry contains data about the entity (if any) registered at the index.
    */
    inline EntryType &GetEntry(const uint32_t index);

private:

    static const uint32_t ENTRIES_PER_PAGE = 1024;  ///< Number of entries in each allocated page (power of two!).
    static const uint32_t MAX_PAGES = 1024;         ///< Maximum number of allocated pages.

    struct Page
    {
        EntryType mEntries[ENTRIES_PER_PAGE];       ///< Array of entries making up this page.
    };

    Directory(const Directory &other);
    Directory &operator=(const Directory &other);

    mutable Mutex mMutex;                           ///< Ensures thread-safe access to the instance data.
    uint32_t mNextIndex;                            ///< Auto-incremented index to use for next registered entity.
    Page *mPages[MAX_PAGES];                        ///< Pointers to allocated pages.
};


template <class EntryType>
inline Directory<EntryType>::Directory() :
  mMutex(),
  mNextIndex(0)
{
    // Clear the page table.
    for (uint32_t page = 0; page < MAX_PAGES; ++page)
    {
        mPages[page] = 0;
    }
}


template <class EntryType>
inline Directory<EntryType>::~Directory()
{
    IAllocator *const pageAllocator(AllocatorManager::GetCache());

    // Free all pages that were allocated.
    for (uint32_t page = 0; page < MAX_PAGES; ++page)
    {
        if (mPages[page])
        {
            // Destruct and free.
            mPages[page]->~Page();
            pageAllocator->Free(mPages[page], sizeof(Page));            
        }
    }
}


template <class EntryType>
inline uint32_t Directory<EntryType>::Allocate(uint32_t index)
{
    mMutex.Lock();

    // Auto-allocate an index if none was specified.
    if (index == 0)
    {
        // TODO: Avoid in-use indices and re-use freed ones.
        // Skip index zero as it's reserved for use as the null address.
        if (++mNextIndex == MAX_PAGES * ENTRIES_PER_PAGE)
        {
            mNextIndex = 1;
        }

        index = mNextIndex;
    }

    // Allocate the page if it hasn't been allocated already.
    const uint32_t page(index / ENTRIES_PER_PAGE);
    if (mPages[page] == 0)
    {
        IAllocator *const pageAllocator(AllocatorManager::GetCache());
        void *const pageMemory(pageAllocator->AllocateAligned(sizeof(Page), THERON_CACHELINE_ALIGNMENT));

        if (pageMemory)
        {
            mPages[page] = new (pageMemory) Page();
        }
        else
        {
            THERON_FAIL_MSG("Out of memory");
        }
    }

    mMutex.Unlock();

    return index;
}


template <class EntryType>
THERON_FORCEINLINE EntryType &Directory<EntryType>::GetEntry(const uint32_t index)
{
    // Compute the page and offset.
    // TODO: Use a mask?
    const uint32_t page(index / ENTRIES_PER_PAGE);
    const uint32_t offset(index % ENTRIES_PER_PAGE);

    THERON_ASSERT(page < MAX_PAGES);
    THERON_ASSERT(offset < ENTRIES_PER_PAGE);
    THERON_ASSERT(mPages[page]);

    return mPages[page]->mEntries[offset];
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_DIRECTORY_DIRECTORY_H

