// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_MESSAGES_IMESSAGE_H
#define THERON_DETAIL_MESSAGES_IMESSAGE_H


#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/Queue.h>


namespace Theron
{
namespace Detail
{


/**
Interface describing the generic API of the message class template.
*/
class IMessage : public Queue<IMessage>::Node
{
public:

    /**
    Gets the address from which the message was sent.
    // TODO: Force-inline
    */
    inline Address From() const
    {
        return mFrom;
    }

    /**
    Returns the memory block in which this message was allocated.
    */
    THERON_FORCEINLINE void *GetBlock() const
    {
        THERON_ASSERT(mBlock);
        return mBlock;
    }

    /**
    Returns the size in bytes of the memory block in which this message was allocated.
    */
    THERON_FORCEINLINE uint32_t GetBlockSize() const
    {
        THERON_ASSERT(mBlockSize);
        return mBlockSize;
    }

    /**
    Returns the message value as blind data.
    */
    THERON_FORCEINLINE const void *GetMessageData() const
    {
        THERON_ASSERT(mBlock);
        return mBlock;
    }

    /**
    Returns the size in bytes of the message data.
    */
    virtual uint32_t GetMessageSize() const = 0;

    /**
    Returns the name of the message type.
    This uniquely identifies the type of the message value.
    \note Unless explicitly specified to avoid C++ RTTI, message names are null.
    */
    virtual const char *TypeName() const = 0;

    /**
    Allows the message instance to destruct its constructed value object before being freed.
    */
    virtual void Release() = 0;

    /**
    Virtual destructor.
    */
    virtual ~IMessage()
    {
    }

protected:

    /**
    Constructs an IMessage.
    \param from The address from which the message was sent.
    \param block The memory block containing the message.
    \param blockSize The size of the memory block containing the message.
    \param typeName String identifier uniquely identifying the type of the message value.
    */
    THERON_FORCEINLINE IMessage(
        const Address &from,
        void *const block,
        const uint32_t blockSize) :
      mFrom(from),
      mBlock(block),
      mBlockSize(blockSize)
    {
    }

private:
    
    IMessage(const IMessage &other);
    IMessage &operator=(const IMessage &other);

    const Address mFrom;            ///< The address from which the message was sent.
    void *const mBlock;             ///< Pointer to the memory block containing the message.
    const uint32_t mBlockSize;      ///< Total size of the message memory block in bytes.
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_MESSAGES_IMESSAGE_H
