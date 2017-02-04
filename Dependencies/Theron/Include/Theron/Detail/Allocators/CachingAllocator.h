// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_ALLOCATORS_CACHINGALLOCATOR_H
#define THERON_DETAIL_ALLOCATORS_CACHINGALLOCATOR_H


#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Allocators/Pool.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


struct DefaultCacheTraits
{
    /**
    Default lock type boils away to no locking.
    */
    class LockType
    {
    public:

        THERON_FORCEINLINE void Lock()
        {
        }

        THERON_FORCEINLINE void Unlock()
        {
        }
    };

    /**
    Default alignment type enforces no entry alignment.
    */
    struct AlignType
    {
    };

    static const uint32_t MAX_POOLS = 8;
    static const uint32_t MAX_BLOCKS = 16;
};


/**
A thread-safe caching allocator that caches free memory blocks.
*/
template <class CacheTraits = DefaultCacheTraits>
class CachingAllocator : public Theron::IAllocator
{
public:

    /**
    Default constructor.
    Constructs an uninitialized CachingAllocator referencing no lower-level allocator.
    */
    inline CachingAllocator();

    /**
    Explicit constructor.
    Constructs a CachingAllocator around an externally owned lower-level allocator.
    The CachingAllocator adds caching of small allocations.
    \param allocator Pointer to a lower-level allocator which the cache will wrap.
    */
    inline explicit CachingAllocator(IAllocator *const allocator);

    /**
    Destructor.
    */
    inline virtual ~CachingAllocator();

    /**
    Sets the internal allocator which is wrapped, or cached, by the caching allocator.
    \note This should only be called at start-of-day before any calls to Allocate.
    */
    inline void SetAllocator(IAllocator *const allocator);

    /**
    Gets the internal allocator which is wrapped, or cached, by the caching allocator.
    */
    inline IAllocator *GetAllocator() const;

    /**
    Allocates a memory block of the given size.
    */
    inline virtual void *Allocate(const uint32_t size);

    /**
    Allocates a memory block of the given size and alignment.
    */
    inline virtual void *AllocateAligned(const uint32_t size, const uint32_t alignment);

    /**
    Frees a previously allocated memory block.
    */
    inline virtual void Free(void *const block);

    /**
    Frees a previously allocated memory block of a known size.
    */
    inline virtual void Free(void *const block, const uint32_t size);

    /**
    Frees all currently cached memory blocks.
    */
    inline void Clear();

private:

    class Entry
    {
    public:

        typedef Detail::Pool<CacheTraits::MAX_BLOCKS> PoolType;

        inline Entry() : mBlockSize(0)
        {
        }

        typename CacheTraits::AlignType mAlign;
        uint32_t mBlockSize;
        PoolType mPool;
    };

    CachingAllocator(const CachingAllocator &other);
    CachingAllocator &operator=(const CachingAllocator &other);

    IAllocator *mAllocator;                     ///< Pointer to a wrapped low-level allocator.
    typename CacheTraits::LockType mLock;       ///< Protects access to the pools.
    Entry mEntries[CacheTraits::MAX_POOLS];     ///< Pools of memory blocks of different sizes.
};


template <class CacheTraits>
THERON_FORCEINLINE CachingAllocator<CacheTraits>::CachingAllocator() : mAllocator(0)
{
}


template <class CacheTraits>
THERON_FORCEINLINE CachingAllocator<CacheTraits>::CachingAllocator(IAllocator *const allocator) : mAllocator(allocator)
{
}


template <class CacheTraits>
THERON_FORCEINLINE CachingAllocator<CacheTraits>::~CachingAllocator()
{
    Clear();
}


template <class CacheTraits>
inline void CachingAllocator<CacheTraits>::SetAllocator(IAllocator *const allocator)
{
    mAllocator = allocator;
}


template <class CacheTraits>
inline IAllocator *CachingAllocator<CacheTraits>::GetAllocator() const
{
    return mAllocator;
}


template <class CacheTraits>
inline void *CachingAllocator<CacheTraits>::Allocate(const uint32_t size)
{
    // Assume word-size alignment by default.
    // We pay the hit of an extra call in this uncommon case.
    return AllocateAligned(size, sizeof(void *));
}


template <class CacheTraits>
inline void *CachingAllocator<CacheTraits>::AllocateAligned(const uint32_t size, const uint32_t alignment)
{
    // Clamp small allocations to at least the size of a pointer.
    const uint32_t blockSize(size >= sizeof(void *) ? size : sizeof(void *));
    void *block(0);

    // Alignment values are expected to be powers of two and at least 4 bytes.
    THERON_ASSERT(blockSize >= 4);
    THERON_ASSERT(alignment >= 4);
    THERON_ASSERT((alignment & (alignment - 1)) == 0);

    mLock.Lock();

    // The last pool is reserved and should always be empty.
    THERON_ASSERT(mEntries[CacheTraits::MAX_POOLS - 1].mBlockSize == 0);
    THERON_ASSERT(mEntries[CacheTraits::MAX_POOLS - 1].mPool.Empty());

    // Search each entry in turn for one whose pool contains blocks of the required size.
    // Stop if we reach the first unused entry (marked by a block size of zero).
    uint32_t index(0);
    while (index < CacheTraits::MAX_POOLS)
    {
        Entry &entry(mEntries[index]);
        if (entry.mBlockSize == blockSize)
        {
            // Try to allocate a block from the pool.
            block = entry.mPool.FetchAligned(alignment);
            break;
        }

        // Found the first unused pool or the very last pool?
        if (entry.mBlockSize == 0)
        {
            // Reserve it for blocks of the current size.
            THERON_ASSERT(entry.mPool.Empty());
            entry.mBlockSize = blockSize;
            break;
        }

        ++index;
    }

    // Swap the pool with that of lower index, if it isn't already first.
    // This is a kind of least-recently-requested replacement policy for pools.
    if (index > 0)
    {
        Entry temp(mEntries[index]);
        mEntries[index] = mEntries[index - 1];
        mEntries[index - 1] = temp;
    }

    // If we claimed the last pool then clear the pool which is
    // now last after the swap, so it's available for next time.
    if (index == CacheTraits::MAX_POOLS - 1)
    {
        Entry &entry(mEntries[CacheTraits::MAX_POOLS - 1]);
        entry.mBlockSize = 0;

        while (!entry.mPool.Empty())
        {
            mAllocator->Free(entry.mPool.Fetch(), entry.mBlockSize);
        }
    }

    // Check that the last pool has been left unused and empty.
    THERON_ASSERT(mEntries[CacheTraits::MAX_POOLS - 1].mBlockSize == 0);
    THERON_ASSERT(mEntries[CacheTraits::MAX_POOLS - 1].mPool.Empty());

    mLock.Unlock();

    if (block == 0)
    {
        // Allocate a new block.
        block = mAllocator->AllocateAligned(blockSize, alignment);
    }

    return block;
}


template <class CacheTraits>
inline void CachingAllocator<CacheTraits>::Free(void *const block)
{
    // We don't try to cache blocks of unknown size.
    mAllocator->Free(block);
}


template <class CacheTraits>
inline void CachingAllocator<CacheTraits>::Free(void *const block, const uint32_t size)
{
    // Small allocations are clamped to at least the size of a pointer.
    const uint32_t blockSize(size >= sizeof(void *) ? size : sizeof(void *));
    bool added(false);

    THERON_ASSERT(blockSize >= 4);
    THERON_ASSERT(block);

    mLock.Lock();

    // Search each entry in turn for one whose pool is for blocks of the given size.
    // Stop if we reach the first unused entry (marked by a block size of zero).
    // Don't search the very last pool, since it is reserved for use in swaps.
    uint32_t index(0);
    while (index < CacheTraits::MAX_POOLS && mEntries[index].mBlockSize)
    {
        Entry &entry(mEntries[index]);
        if (entry.mBlockSize == blockSize)
        {
            // Try to add the block to the pool, if it's not already full.
            added = entry.mPool.Add(block);
            break;
        }

        ++index;
    }

    mLock.Unlock();

    if (!added)
    {
        // No pools are assigned to blocks of this size; free it.
        mAllocator->Free(block, blockSize);
    }
}


template <class CacheTraits>
THERON_FORCEINLINE void CachingAllocator<CacheTraits>::Clear()
{
    mLock.Lock();

    // Free any remaining blocks in the pools.
    for (uint32_t index = 0; index < CacheTraits::MAX_POOLS; ++index)
    {
        Entry &entry(mEntries[index]);
        while (!entry.mPool.Empty())
        {
            mAllocator->Free(entry.mPool.Fetch());
        }
    }

    mLock.Unlock();
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_ALLOCATORS_CACHINGALLOCATOR_H
