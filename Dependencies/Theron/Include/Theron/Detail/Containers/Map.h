// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_CONTAINERS_MAP_H
#define THERON_DETAIL_CONTAINERS_MAP_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>


namespace Theron
{
namespace Detail
{


/**
Generic map container mapping unique keys to values.
Supports multiple values with the same key (ie. multimap semantics).
*/
template <class KeyType, class ValueType, class HashType>
class Map
{
public:

    /**
    Node type, which is allocated and owned by the user.
    */
    struct Node : public List<Node>::Node
    {
        THERON_FORCEINLINE Node(const KeyType &key, const ValueType &value) : mKey(key), mValue(value)
        {
        }

        KeyType mKey;
        ValueType mValue;
    };

    // TODO: Make this private.
    struct Bucket : public List<Bucket>::Node
    {
        List<typename Map::Node> mNodeList;
    };

    /**
    Iterates the nodes with a given key, of which there can be any number.
    Each node contains the key (redundantly, in this use) and the associated value.
    */
    class KeyNodeIterator
    {
    public:

        /**
        Default constructor.
        */
        THERON_FORCEINLINE KeyNodeIterator() :
          mNodeList(0),
          mKey(0),
          mIt()
        {
        }

        /**
        Constructor.
        */
        THERON_FORCEINLINE KeyNodeIterator(const Map *const map, const KeyType &key) :
          mNodeList(&map->mBuckets[HashType::Compute(key)].mNodeList),
          mKey(key),
          mIt(mNodeList->GetIterator())
        {
        }

        /**
        Moves to the next enumerated entry, returning false if no further entries exist.
        \note This method must be called once before calling \ref GetValue.
        */
        THERON_FORCEINLINE bool Next()
        {
            THERON_ASSERT(mNodeList);

            // Move once.
            if (!mIt.Next())
            {
                return false;
            }

            // Skip nodes with different keys.
            while (mIt.Get()->mKey != mKey)
            {
                if (!mIt.Next())
                {
                    return false;
                }
            }

            return true;
        }

        /**
        Gets the map node currently enumerated by the iterator.
        \note \ref Next must be called once before calling this method.
        */
        THERON_FORCEINLINE Node *Get() const
        {
            return mIt.Get();
        }

    private:

        const List<Node> *mNodeList;
        KeyType mKey;
        typename List<Node>::Iterator mIt;
    };

    friend class NodeIterator;
    friend class KeyNodeIterator;

    /**
    Default constructor.
    */
    inline Map();

    /**
    Destructor.
    */
    inline ~Map();

    /**
    Inserts a user-allocated node into the map.
    \note Duplicates are permitted and are not checked for.
    \return True, if the node was successfully inserted.
    */
    inline bool Insert(Node *const node);

    /**
    Removes a node from the map, if it is present.
    \return True, if the node was successfully removed.
    */
    inline bool Remove(Node *const node);

    /**
    Returns true if the map contains one or more inserted nodes with the given key.
    */
    inline bool Contains(const KeyType &key) const;

    /**
    Searches the map for an inserted node with the given key and value.
    */
    inline Node *Find(const KeyType &key, const ValueType &value) const;

    /**
    Returns a pointer to the node which is arbitrarily the first node in the map.
    \note Returns null if the map is empty.
    */
    inline Node *Front() const;

    /**
    Returns an iterator enumerating the inserted nodes whose keys match the given key.
    */
    inline KeyNodeIterator GetKeyNodeIterator(const KeyType &key) const;

private:

    Bucket mBuckets[HashType::RANGE];
    List<Bucket> mNonEmptyBuckets;
};


template <class KeyType, class ValueType, class HashType>
inline Map<KeyType, ValueType, HashType>::Map() : mNonEmptyBuckets()
{
}


template <class KeyType, class ValueType, class HashType>
inline Map<KeyType, ValueType, HashType>::~Map()
{
    // Check the map has been emptied otherwise we'll leak the node memory.
    THERON_ASSERT(mNonEmptyBuckets.Empty());
}


template <class KeyType, class ValueType, class HashType>
THERON_FORCEINLINE bool Map<KeyType, ValueType, HashType>::Insert(Node *const node)
{
    const uint32_t hash(HashType::Compute(node->mKey));
    THERON_ASSERT(hash < HashType::RANGE);
    Bucket &bucket(mBuckets[hash]);

    if (bucket.mNodeList.Empty())
    {
        mNonEmptyBuckets.Insert(&bucket);
    }

    bucket.mNodeList.Insert(node);
    return true;
}


template <class KeyType, class ValueType, class HashType>
THERON_FORCEINLINE bool Map<KeyType, ValueType, HashType>::Remove(Node *const node)
{
    const uint32_t hash(HashType::Compute(node->mKey));
    THERON_ASSERT(hash < HashType::RANGE);
    Bucket &bucket(mBuckets[hash]);

    if (bucket.mNodeList.Remove(node))
    {
        if (bucket.mNodeList.Empty())
        {
            mNonEmptyBuckets.Remove(&bucket);
        }

        return true;
    }

    return false;
}


template <class KeyType, class ValueType, class HashType>
THERON_FORCEINLINE bool Map<KeyType, ValueType, HashType>::Contains(const KeyType &key) const
{
    KeyNodeIterator keyNodes(GetKeyNodeIterator(key));
    return keyNodes.Next();
}


template <class KeyType, class ValueType, class HashType>
THERON_FORCEINLINE typename Map<KeyType, ValueType, HashType>::Node *Map<KeyType, ValueType, HashType>::Find(
    const KeyType &key,
    const ValueType &value) const
{
    const uint32_t hash(HashType::Compute(key));
    THERON_ASSERT(hash < HashType::RANGE);
    const Bucket &bucket(mBuckets[hash]);

    typename List<Node>::Iterator nodes(bucket.mNodeList.GetIterator());
    while (nodes.Next())
    {
        Node *const node(nodes.Get());
        if (node->mKey == key && node->mValue == value)
        {
            return node;
        }
    }

    return 0;
}


template <class KeyType, class ValueType, class HashType>
THERON_FORCEINLINE typename Map<KeyType, ValueType, HashType>::Node *Map<KeyType, ValueType, HashType>::Front() const
{
    // Return the front node from the first non-empty bucket.
    if (mNonEmptyBuckets.Empty())
    {
        return 0;
    }

    const Bucket *const bucket(mNonEmptyBuckets.Front());
    
    THERON_ASSERT(!bucket->mNodeList.Empty());
    return bucket->mNodeList.Front();
}


template <class KeyType, class ValueType, class HashType>
inline typename Map<KeyType, ValueType, HashType>::KeyNodeIterator Map<KeyType, ValueType, HashType>::GetKeyNodeIterator(const KeyType &key) const
{
    return KeyNodeIterator(this, key);
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_CONTAINERS_MAP_H
