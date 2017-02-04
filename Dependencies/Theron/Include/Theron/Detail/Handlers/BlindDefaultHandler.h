// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_BLINDDEFAULTHANDLER_H
#define THERON_DETAIL_HANDLERS_BLINDDEFAULTHANDLER_H


#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/Message.h>
#include <Theron/Detail/Handlers/IDefaultHandler.h>


namespace Theron
{


class Actor;


namespace Detail
{


/**
Instantiable class template that remembers a 'blind' default handler function.
A blind handler is one that takes the message as blind data: a void pointer and a size.
*/
template <class ActorType>
class BlindDefaultHandler : public IDefaultHandler
{
public:

    /**
    Pointer to a member function of the actor type, which is the handler.
    */
    typedef void (ActorType::*HandlerFunction)(const void *const data, const uint32_t size, const Address from);

    /**
    Constructor.
    */
    THERON_FORCEINLINE explicit BlindDefaultHandler(HandlerFunction function) : mHandlerFunction(function)
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~BlindDefaultHandler()
    {
    }

    /**
    Handles the given message.
    \note The message is not consumed by the handler; just acted on or ignored.
    The message will be automatically destroyed when all handlers have seen it.
    */
    inline virtual void Handle(Actor *const actor, const IMessage *const message) const;

private:

    BlindDefaultHandler(const BlindDefaultHandler &other);
    BlindDefaultHandler &operator=(const BlindDefaultHandler &other);

    const HandlerFunction mHandlerFunction;     ///< Pointer to a handler member function on the actor.
};


template <class ActorType>
inline void BlindDefaultHandler<ActorType>::Handle(Actor *const actor, const IMessage *const message) const
{
    THERON_ASSERT(actor);
    THERON_ASSERT(message);
    THERON_ASSERT(mHandlerFunction);

    // Call the handler, passing it the from address and also the message as blind data.
    ActorType *const typedActor = static_cast<ActorType *>(actor);

    const void *const messageData(message->GetMessageData());
    const uint32_t messageSize(message->GetMessageSize());
    const Theron::Address from(message->From());

    THERON_ASSERT(messageData && messageSize);

    (typedActor->*mHandlerFunction)(messageData, messageSize, from);
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_BLINDDEFAULTHANDLER_H
