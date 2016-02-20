// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_FALLBACKHANDLERCOLLECTION_H
#define THERON_DETAIL_HANDLERS_FALLBACKHANDLERCOLLECTION_H


#include <new>

#include <Theron/Address.h>
#include <Theron/AllocatorManager.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Handlers/BlindFallbackHandler.h>
#include <Theron/Detail/Handlers/FallbackHandler.h>
#include <Theron/Detail/Handlers/IFallbackHandler.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{
namespace Detail
{


/**
A collection of handlers for messages of different types.
*/
class FallbackHandlerCollection
{
public:

    /**
    Default constructor.
    */
    FallbackHandlerCollection();

    /**
    Destructor.
    */
    ~FallbackHandlerCollection();

    /**
    Sets the handler in the collection.
    */
    template <class ObjectType>
    inline bool Set(
        ObjectType *const handlerObject,
        void (ObjectType::*handler)(const Address from));

    /**
    Sets the handler in the collection.
    */
    template <class ObjectType>
    inline bool Set(
        ObjectType *const handlerObject,
        void (ObjectType::*handler)(const void *const data, const uint32_t size, const Address from));

    /**
    Removes any registered handler from the collection.
    */
    bool Clear();

    /**
    Handles the given message, passing it to each of the handlers in the collection.
    \return True, if one or more of the handlers in the collection handled the message.
    */
    bool Handle(const IMessage *const message);

private:

    FallbackHandlerCollection(const FallbackHandlerCollection &other);
    FallbackHandlerCollection &operator=(const FallbackHandlerCollection &other);

    /**
    Updates the registered handlers with any changes made since the last message was processed.
    \note This function is intentionally not force-inlined since the handlers don't usually change often.
    */
    void UpdateHandlers();

    IFallbackHandler *mHandler;         ///< Currently registered handler (only one is supported).
    IFallbackHandler *mNewHandler;      ///< New registered handler (will replace the registered one on update).
    bool mHandlersDirty;                ///< Flag indicating that the handlers are out of date.
};


template <class ObjectType>
inline bool FallbackHandlerCollection::Set(
    ObjectType *const handlerObject,
    void (ObjectType::*handler)(const Address from))
{
    typedef FallbackHandler<ObjectType> MessageHandlerType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = true;

    // Destroy any previously set new handler.
    // Note that all handler objects are the same size.
    if (mNewHandler)
    {
        mNewHandler->~IFallbackHandler();
        allocator->Free(mNewHandler, sizeof(MessageHandlerType));
        mNewHandler = 0;
    }

    if (handlerObject && handler)
    {
        // Allocate memory for a message handler object.
        void *const memory = allocator->Allocate(sizeof(MessageHandlerType));
        if (memory == 0)
        {
            return false;
        }

        // Construct a handler object to remember the function pointer and message value type.
        mNewHandler = new (memory) MessageHandlerType(handlerObject, handler);
    }

    return true;
}


template <class ObjectType>
inline bool FallbackHandlerCollection::Set(
    ObjectType *const handlerObject,
    void (ObjectType::*handler)(const void *const data, const uint32_t size, const Address from))
{
    typedef BlindFallbackHandler<ObjectType> MessageHandlerType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = true;

    // Destroy any previously set new handler.
    // Note that all handler objects are the same size.
    if (mNewHandler)
    {
        mNewHandler->~IFallbackHandler();
        allocator->Free(mNewHandler, sizeof(MessageHandlerType));
        mNewHandler = 0;
    }

    if (handlerObject && handler)
    {
        // Allocate memory for a message handler object.
        void *const memory = allocator->Allocate(sizeof(MessageHandlerType));
        if (memory == 0)
        {
            return false;
        }

        // Construct a handler object to remember the function pointer and message value type.
        mNewHandler = new (memory) MessageHandlerType(handlerObject, handler);
    }

    return true;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_FALLBACKHANDLERCOLLECTION_H
