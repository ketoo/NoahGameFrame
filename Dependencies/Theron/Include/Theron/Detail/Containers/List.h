// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_CONTAINERS_LIST_H
#define THERON_DETAIL_CONTAINERS_LIST_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


namespace Theron
{
namespace Detail
{


/**
Class template implementing a generic unsorted list.
\note The item type is the node type and is expected to derive from Node.
*/
template <class ItemType>
class List
{
public:

    /**
    Baseclass that adds link members to node types that derive from it.
    */
    class Node
    {
    public:

        Node *mNext;                ///< Pointer to the next item in the queue.
    };

    /**
    Iterator type.
    */
    class Iterator
    {
    public:

        /**
        Default constructor
        Constructs an invalid iterator which must be assigned before being used.
        */
        THERON_FORCEINLINE Iterator() : mNode(0)
        {
        }

        /**
        Constructor
        \param node The list node that the iterator initially references.
        */
        THERON_FORCEINLINE explicit Iterator(Node *const node) : mNode(node)
        {
        }

        /**
        Moves to the next enumerated entry, returning false if no further entries exist.
        \note This method must be called once before calling \ref Get.
        */
        THERON_FORCEINLINE bool Next()
        {
            // On the first call this skips the head node, which is a dummy.
            mNode = mNode->mNext;
            return (mNode != 0);
        }

        /**
        Gets the item currently enumerated by the iterator.
        \note \ref Next must be called once before calling this method.
        */
        THERON_FORCEINLINE ItemType *Get() const
        {
            THERON_ASSERT(mNode);
            return static_cast<ItemType *>(mNode);
        }

    protected:

        Node *mNode;        ///< Pointer to the list node referenced by the iterator.
    };

    /**
    Constructor
    */
    inline List();
    
    /**
    Destructor
    */
    inline ~List();

    /**
    Returns an iterator enumerating the items in the list.
    */
    inline Iterator GetIterator() const;

    /**
    Returns the number of items currently in the list.
    \return The number of items in the list.
    */
    inline uint32_t Size() const;

    /**
    Returns true if there are no items currently in the list.
    */
    inline bool Empty() const;

    /**
    Empties the list, removing all previously inserted items.
    */
    inline void Clear();

    /**
    Adds an item to the list. The new item is referenced by pointer and is not copied.
    No checking for duplicates is performed.
    \param item A pointer to the item to be added to the list.
    */
    inline void Insert(ItemType *const item);

    /**
    Removes the given item from the list, if it is present.
    \param item A pointer to an item which is to be removed, if present in the list.
    \return True if the item was present and successfully removed, otherwise false.
    \note This method searches the list!
    */
    inline bool Remove(ItemType *const item);

    /**
    Returns a pointer to the first item in the list.
    */
    inline ItemType *Front() const;

private:

    List(const List &other);
    List &operator=(const List &other);

    mutable Node mHead;                 ///< Dummy element at the head of the list.
};


template <class ItemType>
THERON_FORCEINLINE List<ItemType>::List()
{
    mHead.mNext = 0;
}


template <class ItemType>
THERON_FORCEINLINE List<ItemType>::~List()
{
    // The list doesn't own the nodes so there's nothing to free.
    Clear();
}


template <class ItemType>
THERON_FORCEINLINE typename List<ItemType>::Iterator List<ItemType>::GetIterator() const
{
    // The iterator skips the first element, which is fine because it's a dummy.
    return Iterator(&mHead);
}


template <class ItemType>
THERON_FORCEINLINE uint32_t List<ItemType>::Size() const
{
    uint32_t count(0);
    Node *node(mHead.mNext);

    while (node)
    {
        ++count;
        node = node->mNext;
    }

    return count;
}


template <class ItemType>
THERON_FORCEINLINE bool List<ItemType>::Empty() const
{
    return (mHead.mNext == 0);
}


template <class ItemType>
THERON_FORCEINLINE void List<ItemType>::Clear()
{
    // Since the nodes are allocated and owned by the caller, we just forget about them.
    mHead.mNext = 0;
}


template <class ItemType>
THERON_FORCEINLINE void List<ItemType>::Insert(ItemType *const item)
{
    item->mNext = mHead.mNext;
    mHead.mNext = item;
}


template <class ItemType>
inline bool List<ItemType>::Remove(ItemType *const item)
{
    // Find the node and the one before it, if any.
    Node *previous(&mHead);
    Node *node(mHead.mNext);

    while (node)
    {
        // Note compared by address not value.
        if (node == item)
        {
            break;
        }

        previous = node;
        node = node->mNext;
    }

    // Node in list?
    if (node)
    {
        // This works because we use a dummy as the head.
        previous->mNext = node->mNext;
        return true;
    }

    // The caller owns the node so we don't need to free it.
    return false;
}


template <class ItemType>
THERON_FORCEINLINE ItemType *List<ItemType>::Front() const
{
    return static_cast<ItemType *>(mHead.mNext);
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_CONTAINERS_LIST_H
