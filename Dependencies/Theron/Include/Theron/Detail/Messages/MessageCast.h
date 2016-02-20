// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_MESSAGECAST_H
#define THERON_DETAIL_HANDLERS_MESSAGECAST_H


#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/Message.h>
#include <Theron/Detail/Messages/MessageTraits.h>


namespace Theron
{
namespace Detail
{


/**
\brief Dynamic cast utility for message pointers.
A cast utility that can be used to dynamically cast a message of unknown type
to a message of a known type at runtime, using stored runtime type information.
If the unknown message is of the target type then the cast succeeds and a pointer
to the typecast message is returned, otherwise a null pointer is returned.

This utility roughly mimics the functionality of dynamic_cast, but includes
two alternate implementations: one that uses dynamic_cast directly and another
that rolls its own runtime type information only for message classes. The
advantage of the second implementation is that the storage overhead of the extra
runtime type information is not imposed on \em all classes, as with the C++ RTTI.
If the second implementation is used consistently then dynamic_cast is not
called at all, using a partial template specialization trick, and the C++ RTTI
functionality can be turned off (usually by means of a compiler option).

\note Partial template specialization is used here as a device to avoid
introducing a hardcoded dependency on C++ RTTI. The dependency can be avoided
by defining non-zero type IDs for all message types used in the application.

\tparam HAS_TYPE_ID A flag indicating whether the message type has a name.
*/
template <bool HAS_TYPE_ID>
class MessageCast
{
public:

    /**
    \brief Attempts to convert a given message, of unknown type, to one of a target type.
    Returns a null pointer if the message is of the wrong type.
    \tparam ValueType The value type of the target message.
    \param message A pointer to the message of unknown type.
    \return A pointer to the converted message, or null if the types don't match.
    */
    template <class ValueType>
    THERON_FORCEINLINE static const Message<ValueType> *CastMessage(const IMessage *const message)
    {
        THERON_ASSERT(message);

        // If explicit type names are used then they must be defined for all message types.
        THERON_ASSERT_MSG(message->TypeName(), "Message type has null type name");

        // Check the type of the message using the type name it carries, which was set on creation.
        if (message->TypeName() == MessageTraits<ValueType>::TYPE_NAME)
        {
            // Hard-convert the given message to the indicated type.
            return reinterpret_cast<const Message<ValueType> *>(message);
        }

        return 0;
    }
};


// Specialization of MessageCast for the case where the message has no type name.
// This specialization uses C++ built-in RTTI instead of the explicitly stored type names.
template <>
class MessageCast<false>
{
public:

    /**
    Attempts to read a value of the given type from a given message.
    \note Returns a null pointer if the message is of the wrong type.
    */
    template <class ValueType>
    THERON_FORCEINLINE static const Message<ValueType> *CastMessage(const IMessage *const message)
    {
        THERON_ASSERT(message);

#if THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS
        // We're running the specialization of this class that's used for messages without
        // registered type ids/names, so this message hasn't been registered.
        THERON_FAIL_MSG("Message type is not registered");
#endif // THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS

        // Explicit type IDs must be defined for all message types or none at all.
        THERON_ASSERT_MSG(message->TypeName() == 0, "Only some message types are registered!");

        // Try to convert the given message to a message of the expected type.
        // The dynamic_cast used here requires Runtime Type Information (RTTI) support.
        // If you see a failure here then check RTTI is enabled in your build,
        // or if you actually intend to turn it off then register your message type name
        // with THERON_REGISTER_MESSAGE() -- see the RegisteringMessages sample.
        // That causes the default (unspecialized) version of this class to be used,
        // which doesn't try to use dynamic_cast so doesn't require RTTI.
        return dynamic_cast<const Message<ValueType> *>(message);
    }
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_MESSAGECAST_H
