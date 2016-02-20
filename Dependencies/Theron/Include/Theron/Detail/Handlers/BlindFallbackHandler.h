// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_BLINDFALLBACKHANDLER_H
#define THERON_DETAIL_HANDLERS_BLINDFALLBACKHANDLER_H


#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Handlers/IFallbackHandler.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{
namespace Detail
{


/**
Instantiable class template that remembers a 'blind' fallback message handler function
registered with a framework and called for messages that are undelivered or unhandled.
A blind handler is one that takes the message as blind data: a void pointer and a size.
\tparam ObjectType The class on which the handler function is a method.
*/
template <class ObjectType>
class BlindFallbackHandler : public IFallbackHandler
{
public:

    /**
    Pointer to a member function of a handler object.
    */
    typedef void (ObjectType::*HandlerFunction)(const void *const data, const uint32_t size, const Address from);

    /**
    Constructor.
    */
    THERON_FORCEINLINE BlindFallbackHandler(ObjectType *const object, HandlerFunction function) :
      mObject(object),
      mHandlerFunction(function)
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~BlindFallbackHandler()
    {
    }

    /**
    Handles the given message.
    */
    inline virtual void Handle(const IMessage *const message) const
    {
        THERON_ASSERT(mObject);
        THERON_ASSERT(mHandlerFunction);
        THERON_ASSERT(message);

        // Call the handler, passing it the from address and also the message as blind data.
        const void *const messageData(message->GetMessageData());
        const uint32_t messageSize(message->GetMessageSize());
        const Theron::Address from(message->From());

        THERON_ASSERT(messageData && messageSize);

        (mObject->*mHandlerFunction)(messageData, messageSize, from);
    }

private:

    BlindFallbackHandler(const BlindFallbackHandler &other);
    BlindFallbackHandler &operator=(const BlindFallbackHandler &other);

    ObjectType *mObject;                        ///< Pointer to the object owning the handler function.
    const HandlerFunction mHandlerFunction;     ///< Pointer to the handler member function on the owning object.
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_BLINDFALLBACKHANDLER_H
