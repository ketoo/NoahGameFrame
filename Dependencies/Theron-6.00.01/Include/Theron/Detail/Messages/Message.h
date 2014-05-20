// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_MESSAGES_MESSAGE_H
#define THERON_DETAIL_MESSAGES_MESSAGE_H


#include <new>

#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Alignment/MessageAlignment.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/MessageSize.h>
#include <Theron/Detail/Messages/MessageTraits.h>


namespace Theron
{
namespace Detail
{


/**
Message class, used for sending data between actors.
*/
template <class ValueType>
class Message : public IMessage
{
public:

    typedef Message<ValueType> ThisType;

    /**
    Virtual destructor.
    */
    THERON_FORCEINLINE virtual ~Message()
    {
    }

    /**
    Returns the memory block size required to initialize a message of this type.
    */
    THERON_FORCEINLINE static uint32_t GetSize()
    {
        // We allocate an aligned buffer to hold the message and its copy of the value.
        // We lay the message and its value side by side in memory.
        // The value is first, since it's the value that needs the alignment.
        // The message object itself doesn't need special alignment.
        return MessageSize<ValueType>::GetSize() + sizeof(ThisType);
    }

    /**
    Returns the memory block alignment required to initialize a message of this type.
    */
    THERON_FORCEINLINE static uint32_t GetAlignment()
    {
        return MessageAlignment<ValueType>::ALIGNMENT;
    }

    /**
    Initializes a message of this type in the provided memory block.
    The block is allocated and freed by the caller.
    */
    THERON_FORCEINLINE static ThisType *Initialize(void *const block, const ValueType &value, const Address &from)
    {
        THERON_ASSERT(block);

        // Instantiate a new instance of the value type in aligned position at the start of the buffer.
        // We assume that the message value type can be copy-constructed.
        // Messages are explicitly copied to avoid shared memory.
        ValueType *const pValue = new (block) ValueType(value);

        // Allocate the message object immediately after the value, passing it the value's address.
        char *const pObject(reinterpret_cast<char *>(pValue) + MessageSize<ValueType>::GetSize());
        return new (pObject) ThisType(pValue, from);
    }

    /**
    Returns the name of the message type.
    This uniquely identifies the type of the message value.
    \note Unless explicitly specified to avoid C++ RTTI, message names are null.
    */
    virtual const char *TypeName() const
    {
        return MessageTraits<ValueType>::TYPE_NAME;
    }

    /**
    Allows the message instance to destruct its constructed value object before being freed.
    */
    virtual void Release()
    {
        // The referenced block owned by this message is blind data, but we know it holds
        // an instance of the value type, that needs to be explicitly destructed.
        // We have to call the destructor manually because we constructed the object in-place.
        Value().~ValueType();
    }

    /**
    Returns the size in bytes of the message value.
    */
    virtual uint32_t GetMessageSize() const
    {
        // Calculate the size of the message value itself. There's no padding between the
        // message value block and the Message object that follows it.
        return GetBlockSize() - static_cast<uint32_t>(sizeof(ThisType));
    }

    /**
    Gets the value carried by the message.
    */
    THERON_FORCEINLINE const ValueType &Value() const
    {
        // The value is stored at the start of the memory block.
        return *reinterpret_cast<const ValueType *>(GetBlock());
    }

private:

    /**
    Private constructor.
    */
    THERON_FORCEINLINE Message(void *const block, const Address &from) :
      IMessage(from, block, ThisType::GetSize())
    {
        THERON_ASSERT(block);
    }

    Message(const Message &other);
    Message &operator=(const Message &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_MESSAGES_MESSAGE_H

