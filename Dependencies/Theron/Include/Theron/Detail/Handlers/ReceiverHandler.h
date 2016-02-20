// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_RECEIVERHANDLER_H
#define THERON_DETAIL_HANDLERS_RECEIVERHANDLER_H


#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Handlers/IReceiverHandler.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/Message.h>
#include <Theron/Detail/Messages/MessageCast.h>
#include <Theron/Detail/Messages/MessageTraits.h>


namespace Theron
{
namespace Detail
{


/**
Instantiable class template that remembers a message handler function and
the type of message it accepts. It is responsible for checking whether
incoming messages are of the type accepted by the handler, and executing the
handler for messages that match.

Incoming messages are cast at runtime to the type of message handled by the
stored handler, and the handler is executed only if the cast succeeds (returns
a non-zero pointer). The dynamic cast operation is specialized to use either
C++ dynamic_cast or a hand-rolled runtime type information (RTTI) system
that avoids introducing the type information into types other than messages.

\tparam ObjectType The class on which the handler function is a method.
\tparam ValueType The type of message handled by the message handler.
*/
template <class ObjectType, class ValueType>
class ReceiverHandler : public IReceiverHandler
{
public:

    /**
    Pointer to a member function of the object type that can handle messages
    with the given value type.
    */
    typedef void (ObjectType::*HandlerFunction)(const ValueType &message, const Address from);

    /**
    Constructor.
    */
    inline ReceiverHandler(ObjectType *const object, HandlerFunction function) :
      mObject(object),
      mHandlerFunction(function)
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~ReceiverHandler()
    {
    }

    /**
    Returns a pointer to the handler function registered by this instance.
    */
    THERON_FORCEINLINE HandlerFunction GetHandlerFunction() const
    {
        return mHandlerFunction;
    }

    /**
    Returns the unique name of the message type handled by this handler.
    */
    inline virtual const char *GetMessageTypeName() const
    {
        return MessageTraits<ValueType>::TYPE_NAME;
    }

    /**
    Handles the given message, if it's of the type accepted by the handler.
    \return True, if the handler handled the message.
    \note The message is not consumed by the handler; just acted on or ignored.
    The message will be automatically destroyed when all handlers have seen it.
    */
    inline virtual bool Handle(const IMessage *const message) const
    {
        typedef MessageCast<MessageTraits<ValueType>::HAS_TYPE_NAME> MessageCaster;
    
        THERON_ASSERT(mObject);
        THERON_ASSERT(mHandlerFunction);
        THERON_ASSERT(message);

        // Try to convert the message, of unknown type, to message of the assumed type.
        const Message<ValueType> *const typedMessage = MessageCaster:: template CastMessage<ValueType>(message);
        if (typedMessage)
        {
            // Call the handler, passing it the message value and from address.
            (mObject->*mHandlerFunction)(typedMessage->Value(), typedMessage->From());
            return true;
        }

        return false;
    }

private:

    ReceiverHandler(const ReceiverHandler &other);
    ReceiverHandler &operator=(const ReceiverHandler &other);

    ObjectType *mObject;                        ///< Pointer to the object owning the handler function.
    const HandlerFunction mHandlerFunction;     ///< Pointer to the handler member function on the owning object.
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_RECEIVERHANDLER_H
