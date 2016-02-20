// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_NETWORK_MESSAGEFACTORY_H
#define THERON_DETAIL_NETWORK_MESSAGEFACTORY_H


#include <new>

#include <string.h>
#include <stdlib.h>

#include <Theron/AllocatorManager.h>
#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Containers/Map.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/MessageCreator.h>
#include <Theron/Detail/Messages/MessageSize.h>
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
class MessageFactory
{
public:

    /**
    Constructor.
    */
    inline MessageFactory();

    /**
    Destructor.
    */
    inline ~MessageFactory();

    /**
    Registers a message type with the factory against a given name.
    \note The name is assumed to not already exist in the map. At most one message type can be registered per name.
    \return True, unless an error was encountered.
    */
    template <class ValueType>
    inline bool Register(const String &name);

    /**
    Deregisters a message type with the factory against its previously registered name.
    \note Removes of non-existent pairs are ignored. At most one message type can be registered per name. 
    \return True, unless an error was encountered.
    */
    inline bool Deregister(const String &name);

    /**
    Returns true if the map contains a message type registered against the given name.
    */
    inline bool Contains(const String &name);

    /**
    Builds a message of the type registered under the given name.
    The data block pointed to by the provided data pointer is copied as the message value.
    The provided from address is copied as the from address of the message.
    \note If the map contains no type registered against the given name then the returned pointer is null.
    */
    inline IMessage *Build(
        const String &name,
        const void *const messageData,
        const uint32_t messageSize,
        const Address &from) const;

private:

    /**
    Interface that allows builders for different kinds of messages to be referenced polymorphically.
    */
    class IMessageBuilder
    {
    public:

        virtual ~IMessageBuilder()
        {
        }

        virtual IMessage *Build(
            const void *const messageData,
            const uint32_t messageSize,
            const Address &from) const = 0;
    };

    /**
    Creator class template that remembers the type of a message so is able to build instances.
    */
    template <class ValueType>
    class MessageBuilder : public IMessageBuilder
    {
    public:

        virtual IMessage *Build(
            const void *const messageData,
            const uint32_t messageSize,
            const Address &from) const
        {
            // Allocate the message from the global allocator.
            IAllocator *const messageAllocator(AllocatorManager::GetCache());

            // Reject the message data if it's not the right size.
			const uint32_t expectedMessageSize(MessageSize<ValueType>::GetSize());
			if (messageSize == expectedMessageSize)
            {
                // Treat the given data block as an instance of the type.
                const ValueType *const value(reinterpret_cast<const ValueType *>(messageData));

                // Use the message creator to copy the value into an initialized message.
                IMessage *const message = MessageCreator::Create(
                    messageAllocator,
                    *value,
                    from);

                return message;
            }

            return 0;
        }
    };

    typedef Map<const char *, IMessageBuilder *, StringHash> MessageBuilderMap;

    MessageFactory(const MessageFactory &other);
    MessageFactory &operator=(const MessageFactory &other);

    /**
    Non-force-inlined common sub-function to reduce code bloat.
    */
    inline bool RegisterBuilder(const String &name, IMessageBuilder *const builder);

    mutable SpinLock mSpinLock;         ///< Thread-safe access.
    MessageBuilderMap mMap;             ///< Map of names to typed message creators.
};


inline MessageFactory::MessageFactory() : mSpinLock(), mMap()
{
}


inline MessageFactory::~MessageFactory()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    // Clear the map on destruction.
    while (MessageBuilderMap::Node *const node = mMap.Front())
    {
        mMap.Remove(node);

        node->mValue->~IMessageBuilder();
        allocator->Free(node->mValue);

        node->~Node();
        allocator->Free(node, sizeof(MessageBuilderMap::Node));
    }
}


template <class ValueType>
inline bool MessageFactory::Register(const String &name)
{
    typedef MessageBuilder<ValueType> MessageBuilderType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    // Allocate and construct the builder and node speculatively outside the spinlock.
    void *const builderMemory(allocator->Allocate(sizeof(MessageBuilderType)));
    if (builderMemory == 0)
    {
        return false;
    }

    IMessageBuilder *const builder = new (builderMemory) MessageBuilderType;

    if (RegisterBuilder(name, builder))
    {
        return true;
    }

    builder->~IMessageBuilder();
    allocator->Free(builder);

    return false;
}


inline bool MessageFactory::RegisterBuilder(const String &name, IMessageBuilder *const builder)
{
    IAllocator *const allocator(AllocatorManager::GetCache());
    
    void *const nodeMemory(allocator->Allocate(sizeof(MessageBuilderMap::Node)));
    if (nodeMemory == 0)
    {
        return false;
    }

    MessageBuilderMap::Node *node = new (nodeMemory) MessageBuilderMap::Node(name.GetValue(), builder);

    mSpinLock.Lock();

    // Check for existing pairs with the same key. At most one is allowed.
    if (!mMap.Contains(name.GetValue()) && mMap.Insert(node))
    {
        node = 0;
    }

    mSpinLock.Unlock();

    // Destroy the builder and node if the insert failed.
    if (node == 0)
    {
        return true;
    }

    node->~Node();
    allocator->Free(node, sizeof(MessageBuilderMap::Node));

    return false;
}


inline bool MessageFactory::Deregister(const String &name)
{
    IAllocator *const allocator(AllocatorManager::GetCache());
    mSpinLock.Lock();

    MessageBuilderMap::KeyNodeIterator nodes(mMap.GetKeyNodeIterator(name.GetValue()));
    while (nodes.Next())
    {
        MessageBuilderMap::Node *const node(nodes.Get());

        if (mMap.Remove(node))
        {
            mSpinLock.Unlock();

            node->mValue->~IMessageBuilder();
            allocator->Free(node->mValue);

            node->~Node();
            allocator->Free(node, sizeof(MessageBuilderMap::Node));

            mSpinLock.Lock();
        }

        // Restart the iterator since the deletion may have messed it up.
        nodes = mMap.GetKeyNodeIterator(name.GetValue());
    }

    mSpinLock.Unlock();
    return true;
}


inline bool MessageFactory::Contains(const String &name)
{
    bool result(false);
    mSpinLock.Lock();

    result = mMap.GetKeyNodeIterator(name.GetValue()).Next();

    mSpinLock.Unlock();
    return result;
}


inline IMessage *MessageFactory::Build(
    const String &name,
    const void *const messageData,
    const uint32_t messageSize,
    const Address &from) const
{
    IMessage *message(0);
    mSpinLock.Lock();

    MessageBuilderMap::KeyNodeIterator nodes(mMap.GetKeyNodeIterator(name.GetValue()));
    if (nodes.Next())
    {
        const MessageBuilderMap::Node *const node(nodes.Get());
        IMessageBuilder *const builder(node->mValue);

        message = builder->Build(messageData, messageSize, from);
    }

    mSpinLock.Unlock();
    return message;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_NETWORK_MESSAGEFACTORY_H
