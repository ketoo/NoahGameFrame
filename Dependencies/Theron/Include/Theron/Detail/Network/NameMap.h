// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_NETWORK_NAMEMAP_H
#define THERON_DETAIL_NETWORK_NAMEMAP_H


#include <new>

#include <string.h>
#include <stdlib.h>

#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Containers/Map.h>
#include <Theron/Detail/Network/Index.h>
#include <Theron/Detail/Strings/String.h>
#include <Theron/Detail/Strings/StringHash.h>
#include <Theron/Detail/Threading/SpinLock.h>


namespace Theron
{
namespace Detail
{


/**
Associates string names to mailbox indices.
*/
class NameMap
{
public:

    /**
    Constructor.
    */
    inline NameMap();

    /**
    Destructor.
    */
    inline ~NameMap();

    /**
    Inserts a (name, index) pair into the map.
    \note The name is assumed to not already exist in the map. At most one pair with the given name can be inserted.
    \return True, unless an error was encountered.
    */
    inline bool Insert(const String &name, const Index &index);

    /**
    Removes a previously inserted (name, index) pair with the given name from the map.
    \note At most one pair with the given name can exist in the map. Removes of non-existent pairs are ignored.
    \return True, unless an error was encountered.
    */
    inline bool Remove(const String &name);

    /**
    Returns true if the map contains a (name, index) pair with the given name.
    */
    inline bool Contains(const String &name);

    /**
    Returns the index associated with the given name by a (name, index) pair in the map.
    \note If the map contains no (name, index) pair with the given name then the return value is false.
    */
    inline bool Get(const String &name, Index &index) const;

private:

    typedef Map<const char *, Index, StringHash> NameIndexMap;

    NameMap(const NameMap &other);
    NameMap &operator=(const NameMap &other);

    mutable SpinLock mSpinLock;         ///< Thread-safe access.
    NameIndexMap mMap;                  ///< Multimap of names to indices.
};


inline NameMap::NameMap() : mSpinLock(), mMap()
{
}


inline NameMap::~NameMap()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    // Clear the map on destruction.
    while (NameIndexMap::Node *const node = mMap.Front())
    {
        mMap.Remove(node);

        node->~Node();
        allocator->Free(node, sizeof(NameIndexMap::Node));
    }
}


inline bool NameMap::Insert(const String &name, const Index &index)
{
    IAllocator *const allocator(AllocatorManager::GetCache());
    NameIndexMap::Node *node(0);

    // Allocate and construct the node speculatively outside the spinlock.
    void *const nodeMemory(allocator->Allocate(sizeof(NameIndexMap::Node)));
    if (nodeMemory == 0)
    {
        return false;
    }

    node = new (nodeMemory) NameIndexMap::Node(name.GetValue(), index);

    mSpinLock.Lock();

    // Check for existing pairs with the same key. At most one is allowed.
    THERON_ASSERT(!mMap.Contains(name.GetValue()));
    if (mMap.Insert(node))
    {
        node = 0;
    }

    mSpinLock.Unlock();

    // Destroy the node if the insert failed.
    if (node)
    {
        node->~Node();
        allocator->Free(node, sizeof(NameIndexMap::Node));

        return false;
    }

    return true;
}


inline bool NameMap::Remove(const String &name)
{
    IAllocator *const allocator(AllocatorManager::GetCache());
    mSpinLock.Lock();

    NameIndexMap::KeyNodeIterator nodes(mMap.GetKeyNodeIterator(name.GetValue()));
    while (nodes.Next())
    {
        NameIndexMap::Node *const node(nodes.Get());

        if (mMap.Remove(node))
        {
            mSpinLock.Unlock();

            node->~Node();
            allocator->Free(node, sizeof(NameIndexMap::Node));

            mSpinLock.Lock();
        }

        // Restart the iterator since the deletion may have messed it up.
        nodes = mMap.GetKeyNodeIterator(name.GetValue());
    }

    mSpinLock.Unlock();
    return true;
}


inline bool NameMap::Contains(const String &name)
{
    bool result(false);
    mSpinLock.Lock();

    result = mMap.GetKeyNodeIterator(name.GetValue()).Next();

    mSpinLock.Unlock();
    return result;
}


inline bool NameMap::Get(const String &name, Index &index) const
{
    bool result(false);
    mSpinLock.Lock();

    NameIndexMap::KeyNodeIterator nodes(mMap.GetKeyNodeIterator(name.GetValue()));
    if (nodes.Next())
    {
        const NameIndexMap::Node *const node(nodes.Get());
        index = node->mValue;
        result = true;
    }

    mSpinLock.Unlock();
    return result;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_NETWORK_NAMEMAP_H
