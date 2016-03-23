// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_RECEIVERHANDLERCAST_H
#define THERON_DETAIL_HANDLERS_RECEIVERHANDLERCAST_H


#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Handlers/IReceiverHandler.h>
#include <Theron/Detail/Handlers/ReceiverHandler.h>
#include <Theron/Detail/Messages/MessageTraits.h>


namespace Theron
{
namespace Detail
{


/**
\brief Dynamic cast utility for message handler pointers.
*/
template <class ObjectType, bool HAS_TYPE_NAME>
class ReceiverHandlerCast
{
public:

    /**
    \brief Attempts to convert a given message handler, of unknown type, to one of a target type.
    */
    template <class ValueType>
    THERON_FORCEINLINE static const ReceiverHandler<ObjectType, ValueType> *CastHandler(const IReceiverHandler *const handler)
    {
        THERON_ASSERT(handler);

        // If explicit type names are used then they must be defined for all message types.
        THERON_ASSERT_MSG(handler->GetMessageTypeName(), "Missing type name for message type");

        // Compare the handlers using type names.
        if (handler->GetMessageTypeName() != MessageTraits<ValueType>::TYPE_NAME)
        {
            return 0;
        }

        // Convert the given message handler to a handler for the known type.
        typedef ReceiverHandler<ObjectType, ValueType> HandlerType;
        return reinterpret_cast<const HandlerType *>(handler);
    }
};


// Specialization of the ReceiverHandlerCast for the case where the message type has no type name.
// This specialization uses C++ built-in RTTI instead of explicitly stored type names.
template <class ObjectType>
class ReceiverHandlerCast<ObjectType, false>
{
public:

    /**
    Attempts to convert the given message handler, of unknown message type, to a handler of the given type.
    \note Returns a null pointer if the unknown handler is of the wrong type.
    */
    template <class ValueType>
    THERON_FORCEINLINE static const ReceiverHandler<ObjectType, ValueType> *CastHandler(const IReceiverHandler *const handler)
    {
        THERON_ASSERT(handler);

        // Explicit type names must be defined for all message types or none at all.
        THERON_ASSERT_MSG(handler->GetMessageTypeName() == 0, "Type names specified for only some message types!");

        // Try to convert the given message handler to this type.
        typedef ReceiverHandler<ObjectType, ValueType> HandlerType;
        return dynamic_cast<const HandlerType *>(handler);
    }
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_RECEIVERHANDLERCAST_H
