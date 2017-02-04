// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_FALLBACKHANDLER_H
#define THERON_DETAIL_HANDLERS_FALLBACKHANDLER_H


#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Handlers/IFallbackHandler.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{
namespace Detail
{


/**
Instantiable class template that remembers a fallback message handler function
registered with a framework and called for messages that are undelivered or unhandled.
\tparam ObjectType The class on which the handler function is a method.
*/
template <class ObjectType>
class FallbackHandler : public IFallbackHandler
{
public:

    /**
    Pointer to a member function of a handler object.
    */
    typedef void (ObjectType::*HandlerFunction)(const Address from);

    /**
    Constructor.
    */
    THERON_FORCEINLINE FallbackHandler(ObjectType *const object, HandlerFunction function) :
      mObject(object),
      mHandlerFunction(function)
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~FallbackHandler()
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

        // Call the handler, passing it the from address.
        (mObject->*mHandlerFunction)(message->From());
    }

private:

    FallbackHandler(const FallbackHandler &other);
    FallbackHandler &operator=(const FallbackHandler &other);

    ObjectType *mObject;                        ///< Pointer to the object owning the handler function.
    const HandlerFunction mHandlerFunction;     ///< Pointer to the handler member function on the owning object.
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_FALLBACKHANDLER_H
