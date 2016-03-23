// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_NETWORK_NETWORKMESSAGE_H
#define THERON_DETAIL_NETWORK_NETWORKMESSAGE_H


#include <new>

#include <stdlib.h>
#include <string.h>

#include <Theron/Align.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4996)  // function or variable may be unsafe.
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
Message object used to wrap Theron messages for sending on the network.
*/
class NetworkMessage
{
public:

    /**
    Returns the size in bytes of the memory block required to allocate a NetworkMessage.
    */
    inline static uint32_t GetSize(
        const char *const fromName,
        const char *const toName,
        const char *const messageName,
        const void *const messageData,
        const uint32_t messageSize);

    /**
    Constructs a NetworkMessage in the provided memory buffer.
    */
    inline static NetworkMessage *Initialize(
        void *const memoryBlock,
        const char *const fromName,
        const char *const toName,
        const char *const messageName,
        const void *const messageData,
        const uint32_t messageSize);

    /**
    Gets the 'from' name string.
    */
    inline const char *GetFromName() const;

    /**
    Gets the 'to' name string.
    */
    inline const char *GetToName() const;

    /**
    Gets the message type name string.
    */
    inline const char *GetMessageName() const;

    /**
    Gets a pointer to the message data
    */
    inline const void *GetMessageData() const;

    /**
    Gets the size of the message data in bytes.
    */
    inline uint32_t GetMessageSize() const;

private:

    uint32_t mToNameOffset;         ///< Offset to the null-terminated 'to' name in bytes.
    uint32_t mMessageNameOffset;    ///< Offset to the null-terminated message type name in bytes.
    uint32_t mMessageDataOffset;    ///< Offset to the message data block in bytes.
    uint32_t mMessageDataSize;      ///< Size of the message data block in bytes.
};


inline uint32_t NetworkMessage::GetSize(
    const char *const fromName,
    const char *const toName,
    const char *const messageName,
    const void *const /*messageData*/,
    const uint32_t messageSize)
{
    uint32_t size(0);

    // String sizes including the terminating null characters.
    // The 'from' and 'to' addresses are allowed to be null, and if so are skipped.
    uint32_t fromNameSize(0);
    if (fromName)
    {
        fromNameSize = static_cast<uint32_t>(strlen(fromName) + 1);
    }

    uint32_t toNameSize(0);
    if (toName)
    {
        toNameSize = static_cast<uint32_t>(strlen(toName) + 1);
    }

    const uint32_t messageNameSize(static_cast<uint32_t>(strlen(messageName) + 1));

    size += sizeof(NetworkMessage);
    size += fromNameSize;
    size += toNameSize;
    size += messageNameSize;
    size += messageSize;

    return THERON_ROUNDUP(size, 4);
}


inline NetworkMessage *NetworkMessage::Initialize(
    void *const memoryBlock,
    const char *const fromName,
    const char *const toName,
    const char *const messageName,
    const void *const messageData,
    const uint32_t messageSize)
{
    NetworkMessage *const instance = new (memoryBlock) NetworkMessage;

    // String sizes including the terminating null characters.
    // The 'from' address is allowed to be null, and if so is skipped.
    uint32_t fromNameSize(0);
    if (fromName)
    {
        fromNameSize = static_cast<uint32_t>(strlen(fromName) + 1);
    }

    uint32_t toNameSize(0);
    if (toName)
    {
        toNameSize = static_cast<uint32_t>(strlen(toName) + 1);
    }

    const uint32_t messageNameSize(static_cast<uint32_t>(strlen(messageName) + 1));

    // Initialize the instance members.
    // The offset to the 'from' name is implicit since it's just the size of the class.
    const uint32_t fromNameOffset(sizeof(NetworkMessage));
    instance->mToNameOffset = fromNameOffset + fromNameSize;
    instance->mMessageNameOffset = instance->mToNameOffset + toNameSize;
    instance->mMessageDataOffset = instance->mMessageNameOffset + messageNameSize;
    instance->mMessageDataSize = messageSize;

    // Copy in the actual data.
    uint8_t *data(reinterpret_cast<uint8_t *>(memoryBlock));

    if (fromNameSize)
    {
        memcpy(data + sizeof(NetworkMessage), fromName, fromNameSize);
    }

    if (toNameSize)
    {
        memcpy(data + instance->mToNameOffset, toName, toNameSize);
    }

    memcpy(data + instance->mMessageNameOffset, messageName, messageNameSize);
    memcpy(data + instance->mMessageDataOffset, messageData, messageSize);
    
    return instance;
}


THERON_FORCEINLINE const char *NetworkMessage::GetFromName() const
{
    const uint8_t *const data(reinterpret_cast<const uint8_t *>(this));
    const uint8_t *const field(data + sizeof(NetworkMessage));

    // The 'from' name can be null, in which case it's skipped.
    if (mToNameOffset != sizeof(NetworkMessage))
    {
        return reinterpret_cast<const char *>(field);
    }

    return 0;
}


THERON_FORCEINLINE const char *NetworkMessage::GetToName() const
{
    const uint8_t *const data(reinterpret_cast<const uint8_t *>(this));
    const uint8_t *const field(data + mToNameOffset);

    // The 'to' name can be null, in which case it's skipped.
    if (mMessageNameOffset != mToNameOffset)
    {
        return reinterpret_cast<const char *>(field);
    }

    return 0;
}


THERON_FORCEINLINE const char *NetworkMessage::GetMessageName() const
{
    const uint8_t *const data(reinterpret_cast<const uint8_t *>(this));
    const uint8_t *const field(data + mMessageNameOffset);
    return reinterpret_cast<const char *>(field);
}


THERON_FORCEINLINE const void *NetworkMessage::GetMessageData() const
{
    const uint8_t *const data(reinterpret_cast<const uint8_t *>(this));
    const uint8_t *const field(data + mMessageDataOffset);
    return reinterpret_cast<const void *>(field);
}


THERON_FORCEINLINE uint32_t NetworkMessage::GetMessageSize() const
{
    return mMessageDataSize;
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_NETWORK_NETWORKMESSAGE_H
