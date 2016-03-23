// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_HANDLERCOLLECTION_H
#define THERON_DETAIL_HANDLERS_HANDLERCOLLECTION_H


#include <new>

#include <Theron/Address.h>
#include <Theron/AllocatorManager.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Handlers/IMessageHandler.h>
#include <Theron/Detail/Handlers/MessageHandler.h>
#include <Theron/Detail/Handlers/MessageHandlerCast.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/MessageTraits.h>
#include <Theron/Detail/Scheduler/IScheduler.h>
#include <Theron/Detail/Scheduler/MailboxContext.h>


namespace Theron
{


class Actor;


namespace Detail
{


/**
A collection of handlers for messages of different types.
*/
class HandlerCollection
{
public:

    /**
    Default constructor.
    */
    HandlerCollection();

    /**
    Destructor.
    */
    ~HandlerCollection();

    /**
    Adds a handler to the collection.
    */
    template <class ActorType, class ValueType>
    inline bool Add(void (ActorType::*handler)(const ValueType &message, const Address from));

    /**
    Removes a handler from the collection, if it is present.
    */
    template <class ActorType, class ValueType>
    inline bool Remove(void (ActorType::*handler)(const ValueType &message, const Address from));

    /**
    Returns true if the given handler is registered.
    */
    template <class ActorType, class ValueType>
    inline bool Contains(void (ActorType::*handler)(const ValueType &message, const Address from)) const;

    /**
    Unregisters all registered handlers.
    */
    inline bool Clear();

    /**
    Handles the given message, passing it to each of the handlers in the collection.
    \return True, if one or more of the handlers in the collection handled the message.
    */
    inline bool Handle(
        MailboxContext *const mailboxContext,
        Actor *const actor,
        const IMessage *const message);

private:

    typedef List<IMessageHandler> MessageHandlerList;

    HandlerCollection(const HandlerCollection &other);
    HandlerCollection &operator=(const HandlerCollection &other);

    /**
    Updates the registered handlers with any changes made since the last message was processed.
    \note This function is intentionally not force-inlined since the handlers don't usually change often.
    */
    void UpdateHandlers();

    MessageHandlerList mHandlers;       ///< List of handlers in the collection.
    MessageHandlerList mNewHandlers;    ///< List of handlers added since last update.
    bool mHandlersDirty;                ///< Flag indicating that the handlers are out of date.
};


template <class ActorType, class ValueType>
THERON_FORCEINLINE bool HandlerCollection::Add(void (ActorType::*handler)(const ValueType &message, const Address from))
{
    typedef MessageHandler<ActorType, ValueType> MessageHandlerType;

    IAllocator *const allocator(AllocatorManager::GetCache());

    // Allocate memory for a message handler object.
    void *const memory = allocator->Allocate(sizeof(MessageHandlerType));
    if (memory == 0)
    {
        return false;
    }

    // Construct a handler object to remember the function pointer and message value type.
    MessageHandlerType *const messageHandler = new (memory) MessageHandlerType(handler);

    // We don't check for duplicates because multiple registrations of the same function are allowed.
    mNewHandlers.Insert(messageHandler);
    mHandlersDirty = true;

    return true;
}


template <class ActorType, class ValueType>
THERON_FORCEINLINE bool HandlerCollection::Remove(void (ActorType::*handler)(const ValueType &message, const Address from))
{
    // If the message value type has a valid (non-zero) type name defined for it,
    // then we use explicit type names to match messages to handlers.
    // The default value of zero indicates that no type name has been defined,
    // in which case we rely on compiler-generated RTTI to identify message types.
    typedef MessageHandler<ActorType, ValueType> MessageHandlerType;
    typedef MessageHandlerCast<ActorType, MessageTraits<ValueType>::HAS_TYPE_NAME> HandlerCaster;

    // We don't need to lock this because only one thread can access it at a time.
    // Find the handler in the registered handler list.
    typename MessageHandlerList::Iterator handlers(mHandlers.GetIterator());
    while (handlers.Next())
    {
        IMessageHandler *const messageHandler(handlers.Get());

        // Try to convert this handler, of unknown type, to the target type.
        if (const MessageHandlerType *const typedHandler = HandlerCaster:: template CastHandler<ValueType>(messageHandler))
        {
            // Don't count the handler if it's already marked for deregistration.
            if (typedHandler->GetHandlerFunction() == handler && !typedHandler->IsMarked())
            {
                // Mark the handler for deregistration.
                // We defer the actual deregistration and deletion until
                // after all active message handlers have been executed, because
                // message handlers themselves can deregister handlers.
                messageHandler->Mark();
                mHandlersDirty = true;

                return true;
            }
        }
    }

    // The handler wasn't in the registered list, but maybe it's in the new handlers list.
    // That can happen if the handler was only just registered prior to this in the same function.
    // It's a bit weird to register a handler and then immediately deregister it, but legal.
    handlers = mNewHandlers.GetIterator();
    while (handlers.Next())
    {
        IMessageHandler *const messageHandler(handlers.Get());

        // Try to convert this handler, of unknown type, to the target type.
        if (const MessageHandlerType *const typedHandler = HandlerCaster:: template CastHandler<ValueType>(messageHandler))
        {
            // Don't count the handler if it's already marked for deregistration.
            if (typedHandler->GetHandlerFunction() == handler && !typedHandler->IsMarked())
            {
                // Mark the handler for deregistration.
                messageHandler->Mark();
                mHandlersDirty = true;

                return true;
            }
        }
    }

    return false;
}


template <class ActorType, class ValueType>
THERON_FORCEINLINE bool HandlerCollection::Contains(void (ActorType::*handler)(const ValueType &message, const Address from)) const
{
    typedef MessageHandler<ActorType, ValueType> MessageHandlerType;
    typedef MessageHandlerCast<ActorType, MessageTraits<ValueType>::HAS_TYPE_NAME> HandlerCaster;

    // Search for the handler in the registered handler list.
    typename MessageHandlerList::Iterator handlers(mHandlers.GetIterator());
    while (handlers.Next())
    {
        IMessageHandler *const messageHandler(handlers.Get());

        // Try to convert this handler, of unknown type, to the target type.
        if (const MessageHandlerType *const typedHandler = HandlerCaster:: template CastHandler<ValueType>(messageHandler))
        {
            // Count as not registered if it's marked for deregistration.
            // But it may be registered more than once, so keep looking.
            if (typedHandler->GetHandlerFunction() == handler && !typedHandler->IsMarked())
            {
                return true;
            }
        }
    }

    // The handler wasn't in the registered list, but maybe it's in the new handlers list.
    handlers = mNewHandlers.GetIterator();
    while (handlers.Next())
    {
        IMessageHandler *const messageHandler(handlers.Get());

        // Try to convert this handler, of unknown type, to the target type.
        if (const MessageHandlerType *const typedHandler = HandlerCaster:: template CastHandler<ValueType>(messageHandler))
        {
            // Count as not registered if it's marked for deregistration.
            // But it may be registered more than once, so keep looking.
            if (typedHandler->GetHandlerFunction() == handler && !typedHandler->IsMarked())
            {
                return true;
            }
        }
    }

    return false;
}


THERON_FORCEINLINE bool HandlerCollection::Clear()
{
    IAllocator *const allocator(AllocatorManager::GetCache());

    // Free all currently allocated handler objects.
    while (IMessageHandler *const handler = mHandlers.Front())
    {
        mHandlers.Remove(handler);
        handler->~IMessageHandler();
        allocator->Free(handler);
    }

    while (IMessageHandler *const handler = mNewHandlers.Front())
    {
        mNewHandlers.Remove(handler);
        handler->~IMessageHandler();
        allocator->Free(handler);
    }

    mHandlersDirty = false;
    return true;
}


THERON_FORCEINLINE bool HandlerCollection::Handle(
    MailboxContext *const mailboxContext,
    Actor *const actor,
    const IMessage *const message)
{
    bool handled(false);
    IScheduler *const scheduler(mailboxContext->mScheduler);

    THERON_ASSERT(scheduler);
    THERON_ASSERT(actor);
    THERON_ASSERT(message);

    // Update the handler list if there have been changes.
    if (mHandlersDirty)
    {
        UpdateHandlers();
    }

    // Give each registered handler a chance to handle this message.
    MessageHandlerList::Iterator handlers(mHandlers.GetIterator());
    while (handlers.Next())
    {
        IMessageHandler *const messageHandler(handlers.Get());

        // We notify the scheduler, which acts as an observer.
        scheduler->BeginHandler(mailboxContext, messageHandler);
        handled |= messageHandler->Handle(actor, message);
        scheduler->EndHandler(mailboxContext, messageHandler);
    }

    return handled;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_HANDLERCOLLECTION_H
