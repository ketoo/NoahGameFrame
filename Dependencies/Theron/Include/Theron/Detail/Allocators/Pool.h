// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_ALLOCATORS_POOL_H
#define THERON_DETAIL_ALLOCATORS_POOL_H


#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
A pool of free memory blocks.
*/
template <uint32_t MAX_BLOCKS>
class Pool
{
public:

    /**
    Constructor.
    */
    inline Pool();

    /**
    Returns true if the pool contains no memory blocks.
    */
    inline bool Empty() const;

    /**
    Adds a memory block to the pool.
    */
    inline bool Add(void *memory);

    /**
    Retrieves a memory block from the pool with the given alignment.
    \return Zero if no suitable blocks in pool.
    */
    inline void *FetchAligned(const uint32_t alignment);

    /**
    Retrieves a memory block from the pool with any alignment.
    \return Zero if no blocks in pool.
    */
    inline void *Fetch();

private:

    /**
    A node representing a free memory block within the pool.
    Nodes are created in-place within the free blocks they represent.
    */
    struct Node
    {
        THERON_FORCEINLINE Node() : mNext(0)
        {
        }

        Node *mNext;                        ///< Pointer to next node in a list.
    };

    Node mHead;                             ///< Dummy node at head of a linked list of nodes in the pool.
    uint32_t mBlockCount;                   ///< Number of blocks currently cached in the pool.
};


template <uint32_t MAX_BLOCKS>
THERON_FORCEINLINE Pool<MAX_BLOCKS>::Pool() :
  mHead(),
  mBlockCount(0)
{
}


template <uint32_t MAX_BLOCKS>
THERON_FORCEINLINE bool Pool<MAX_BLOCKS>::Empty() const
{
    THERON_ASSERT((mBlockCount == 0 && mHead.mNext == 0) || (mBlockCount != 0 && mHead.mNext != 0));
    return (mBlockCount == 0);
}


template <uint32_t MAX_BLOCKS>
THERON_FORCEINLINE bool Pool<MAX_BLOCKS>::Add(void *const memory)
{
    THERON_ASSERT(memory);

    // Below maximum block count limit?
    if (mBlockCount < MAX_BLOCKS)
    {
        // Just call it a node and link it in.
        Node *const node(reinterpret_cast<Node *>(memory));

        node->mNext = mHead.mNext;
        mHead.mNext = node;

        ++mBlockCount;
        return true;
    }

    return false;
}


template <uint32_t MAX_BLOCKS>
THERON_FORCEINLINE void *Pool<MAX_BLOCKS>::FetchAligned(const uint32_t alignment)
{
    Node *previous(&mHead);
    const uint32_t alignmentMask(alignment - 1);

    // Search the block list.
    Node *node(mHead.mNext);
    while (node)
    {
        Node *const next(node->mNext);

        // This is the THERON_ALIGNED macro with the alignment mask calculated once outside the loop.
        if ((reinterpret_cast<uintptr_t>(node) & alignmentMask) == 0)
        {
            // Remove from list and return as a block.
            previous->mNext = next;
            --mBlockCount;

            return reinterpret_cast<void *>(node);
        }

        previous = node;
        node = next;
    }

    // Zero result indicates no correctly aligned block available.
    return 0;
}


template <uint32_t MAX_BLOCKS>
THERON_FORCEINLINE void *Pool<MAX_BLOCKS>::Fetch()
{
    // Grab first block in the list if the list isn't empty.
    Node *const node(mHead.mNext);
    if (node)
    {
        mHead.mNext = node->mNext;
        --mBlockCount;

        return reinterpret_cast<void *>(node);
    }

    // Zero result indicates no block available.
    return 0;
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_ALLOCATORS_POOL_H
