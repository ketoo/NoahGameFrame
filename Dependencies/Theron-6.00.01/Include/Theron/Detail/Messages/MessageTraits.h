// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_MESSAGES_MESSAGETRAITS_H
#define THERON_DETAIL_MESSAGES_MESSAGETRAITS_H


namespace Theron
{
namespace Detail
{


/**
\brief Traits template that stores meta-information about message types.

The MessageTraits template can be specialized for individual message types
in order to label the types with their string names.
By storing the name of the message type in every sent message, Theron is
able to match the type with the types expected by message handlers
registered in the receiving actor.

The default implementation defines a null pointer (no name) for all types.
The null pointer is a reserved value and implies that the type has no explicit name.
Types with null names are matched by means of dynamic_cast, which
relies on RTTI (Runtime Type Information - the automatic storing of a
type identifier in every class). By default Theron uses RTTI exclusively.

The availability of RTTI is a compilation option, usually on by default
and disabled by means of an optional compiler flag. It introduces a small
size overhead into every class due to the need to store a hidden identifier to
identify the class's type. Note that this overhead is applied to all classes
and not just messages within Theron. The overhead is not normally a problem,
however in applications with tightly constrained memory requirements (such as
embedded environments and games consoles) it is undesirable.

The dependency on RTTI can be avoided by specifying non-null names
for all message types used in the application. Users can define
specializations of the traits template for their own message types, with
non-zero name pointers. Doing so causes the type names stored with every
sent message to be populated with the registered names of the types.
If names are provided for all message types then the names are used for type
matching, instead of the built-in C++ RTTI. This then allows RTTI to be disabled
in the application build.

The \ref THERON_REGISTER_MESSAGE macro can be used as a shorthand mechanism
for registering automatically-generated names with message types.

\note If type names are used, then unique names must be specified for \em all
message types. Failure to specify a valid, unique name for one or more message
types will result in undefined behavior (usually caught by asserts). Although
type names must be unique and non-null, their values are arbitrary.

\tparam ValueType The message type for which the traits are defined.
\see THERON_REGISTER_MESSAGE
*/
template <class ValueType>
struct MessageTraits
{
    /**
    \brief Indicates whether the message type has an explicit name.
    Message types for which have valid type names are identified
    using their names rather than with built-in C++ Runtime Type Information
    (RTTI) via dynamic_cast.
    */
    static const bool HAS_TYPE_NAME = false;
    
    /**
    \brief The unique name of the type.
    */
    static const char *const TYPE_NAME;
};


template <class ValueType>
const char *const MessageTraits<ValueType>::TYPE_NAME = 0;


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_MESSAGES_MESSAGETRAITS_H
