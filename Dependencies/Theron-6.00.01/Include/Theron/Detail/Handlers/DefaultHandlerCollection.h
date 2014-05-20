// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_DEFAULTHANDLERCOLLECTION_H
#define THERON_DETAIL_HANDLERS_DEFAULTHANDLERCOLLECTION_H


#include <new>

#include <Theron/Address.h>
#include <Theron/AllocatorManager.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Handlers/BlindDefaultHandler.h>
#include <Theron/Detail/Handlers/DefaultHandler.h>
#include <Theron/Detail/Handlers/IDefaultHandler.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{


class Actor;


namespace Detail
{


/**
A collection of default message handlers.
\note The 'collection' only holds one handler.
*/
class DefaultHandlerCollection
{
public:

    /**
    Default constructor.
    */
    DefaultHandlerCollection();

    /**
    Destructor.
    */
    ~DefaultHandlerCollection();

    /**
    Sets the handler in the collection.
    */
    template <class ActorType>
    inline bool Set(void (ActorType::*handler)(const Address from));

    /**
    Sets the handler in the collection.
    */
    template <class ActorType>
    inline bool Set(void (ActorType::*handler)(const void *const data, const uint32_t size, const Address from));

    /**
    Removes any registered handler from the collection.
    */
    bool Clear();

    /**
    Handles the given message, passing it to each of the handlers in the collection.
    \return True, if one or more of the handlers in the collection handled the message.
    */
    bool Handle(Actor *const actor, const IMessage *const message);

private:

    DefaultHandlerCollection(const DefaultHandlerCollection &other);
    DefaultHandlerCollection &operator=(const DefaultHandlerCollection &other);

    /**
    Updates the registered handlers with any changes made since the last message was processed.
    \note This function is intentionally not force-inlined since the handlers don't usually change often.
    */
    void UpdateHandlers();

    IDefaultHandler *mHandler;          ///< Currently registered handler (only one is supported).
    IDefaultHandler *mNewHandler;       ///< New registered handler (will replace the registered one on update).
    bool mHandlersDirty;                ///< Flag indicating that the handlers are out of date.
};


template <class ActorType>
inline bool DefaultHandlerCollection::Set(void (ActorType::*handler)(const Address from))
{
    typedef DefaultHandler<ActorType> MessageHandlerType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = true;

    // Destroy any previously set new handler.
    // Note that all handler objects are the same size.
    if (mNewHandler)
    {
        mNewHandler->~IDefaultHandler();
        allocator->Free(mNewHandler, sizeof(MessageHandlerType));
        mNewHandler = 0;
    }

    if (handler)
    {
        // Allocate memory for a message handler object.
        void *const memory = allocator->Allocate(sizeof(MessageHandlerType));
        if (memory == 0)
        {
            return false;
        }

        // Construct a handler object to remember the function pointer and message value type.
        mNewHandler = new (memory) MessageHandlerType(handler);
    }

    return true;
}


template <class ActorType>
inline bool DefaultHandlerCollection::Set(void (ActorType::*handler)(const void *const data, const uint32_t size, const Address from))
{
    typedef BlindDefaultHandler<ActorType> MessageHandlerType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    mHandlersDirty = true;

    // Destroy any previously set new handler.
    // Note that all handler objects are the same size.
    if (mNewHandler)
    {
        mNewHandler->~IDefaultHandler();
        allocator->Free(mNewHandler, sizeof(MessageHandlerType));
        mNewHandler = 0;
    }

    if (handler)
    {
        // Allocate memory for a message handler object.
        void *const memory = allocator->Allocate(sizeof(MessageHandlerType));
        if (memory == 0)
        {
            return false;
        }

        // Construct a handler object to remember the function pointer and message value type.
        mNewHandler = new (memory) MessageHandlerType(handler);
    }

    return true;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_DEFAULTHANDLERCOLLECTION_H
