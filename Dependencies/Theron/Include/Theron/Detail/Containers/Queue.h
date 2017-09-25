// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_CONTAINERS_QUEUE_H
#define THERON_DETAIL_CONTAINERS_QUEUE_H


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
A fast unbounded queue.

The queue is unbounded and internally is a doubly-linked, intrusive linked list.
The head and tail are stored as 'dummy' nodes to simplify insertion and removal.
The head and tail are separate nodes to separate the concerns of pushers and poppers.

\note The queue is intrusive and the item type is expected to derive from Queue<ItemType>::Node.
*/
template <class ItemType>
class Queue
{
public:

    /**
    Baseclass that adds link members to node types that derive from it.
    In order to be used with the queue, item classes must derive from Node.
    */
    class THERON_PREALIGN(THERON_CACHELINE_ALIGNMENT) Node
    {
    public:

        inline Node() : mNext(0), mPrev(0)
        {
        }

        Node *mNext;        ///< Pointer to the next item in the list.
        Node *mPrev;        ///< Pointer to the previous item in the list.

    private:

        Node(const Node &other);
        Node &operator=(const Node &other);

    } THERON_POSTALIGN(THERON_CACHELINE_ALIGNMENT);

    /**
    Constructor
    */
    inline Queue();

    /**
    Destructor
    */
    inline ~Queue();

    /**
    Returns true if the queue contains no items.
    Call this before calling Pop or Front.
    */
    inline bool Empty() const;

    /**
    Pushes an item onto the queue.
    */
    inline void Push(ItemType *const item);

    /**
    Peeks at the item at the front of the queue without removing it.
    \note It's illegal to call Front when the queue is empty.
    */
    inline ItemType *Front() const;

    /**
    Removes and returns the item at the front of the queue.
    \note It's illegal to call Pop when the queue is empty.
    */
    inline ItemType *Pop();

private:

    Queue(const Queue &other);
    Queue &operator=(const Queue &other);

    Node mHead;     ///< Dummy node that is always the head of the list.
    Node mTail;     ///< Dummy node that is always the tail of the list.
};


template <class ItemType>
THERON_FORCEINLINE Queue<ItemType>::Queue() : mHead(), mTail()
{
    mHead.mNext = 0;
    mHead.mPrev = &mTail;

    mTail.mNext = &mHead;
    mTail.mPrev = 0;
}


template <class ItemType>
THERON_FORCEINLINE Queue<ItemType>::~Queue()
{
    // If the queue hasn't been emptied by the caller we'll leak the nodes.
    THERON_ASSERT(mHead.mNext == 0);
    THERON_ASSERT(mHead.mPrev == &mTail);

    THERON_ASSERT(mTail.mNext == &mHead);
    THERON_ASSERT(mTail.mPrev == 0);
}


template <class ItemType>
THERON_FORCEINLINE bool Queue<ItemType>::Empty() const
{
    return (mHead.mPrev == &mTail);
}


template <class ItemType>
THERON_FORCEINLINE void Queue<ItemType>::Push(ItemType *const item)
{

#if THERON_DEBUG

    // Check that the pushed item isn't already in the queue.
    for (Node *node(mHead.mPrev); node != &mTail; node = node->mPrev)
    {
        THERON_ASSERT(node != item);
    }

#endif

    // Doubly-linked list insert at back, ie. in front of the dummy tail.
    item->mPrev = &mTail;
    item->mNext = mTail.mNext;

    mTail.mNext->mPrev = item;
    mTail.mNext = item;
}


template <class ItemType>
THERON_FORCEINLINE ItemType *Queue<ItemType>::Front() const
{
    // It's illegal to call Front when the queue is empty.
    THERON_ASSERT(mHead.mPrev != &mTail);
    return static_cast<ItemType *>(mHead.mPrev);
}


template <class ItemType>
THERON_FORCEINLINE ItemType *Queue<ItemType>::Pop()
{
    Node *const item(mHead.mPrev);

    // It's illegal to call Pop when the queue is empty.
    THERON_ASSERT(item != &mTail);

    // Doubly-linked list remove from front, ie. behind the dummy head.
    item->mPrev->mNext = &mHead;
    mHead.mPrev = item->mPrev;

    return static_cast<ItemType *>(item);
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_CONTAINERS_QUEUE_H
