// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_RECEIVER_H
#define THERON_RECEIVER_H


/**
\file Receiver.h
Utility that can receive messages from actors.
*/


#include <new>

#include <Theron/Address.h>
#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Directory/Entry.h>
#include <Theron/Detail/Handlers/ReceiverHandler.h>
#include <Theron/Detail/Handlers/IReceiverHandler.h>
#include <Theron/Detail/Handlers/ReceiverHandlerCast.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Messages/MessageCreator.h>
#include <Theron/Detail/Messages/MessageTraits.h>
#include <Theron/Detail/Strings/String.h>
#include <Theron/Detail/Strings/StringPool.h>
#include <Theron/Detail/Threading/Atomic.h>
#include <Theron/Detail/Threading/Condition.h>
#include <Theron/Detail/Threading/Lock.h>
#include <Theron/Detail/Threading/Mutex.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{


class EndPoint;
class Framework;


/**
\brief A standalone entity that can accept messages sent by \ref Actor "actors".

A receiver can be instantiated in non-actor user code. Each receiver has
a unique \ref Address "address" not shared by any actor and can receive \ref
Actor::Send "messages sent to it by actors" (usually in response to \ref Framework::Send
"messages sent to those actors" by the non-actor code). The Receiver provides facilities
for synchronizing with the arrival of an expected message, as well as for registering
message handlers to handle and process messages as they arrive.

Every receiver instantiated in user code is automatically assigned its own unique
\ref Address "address", which enables actors to send messages to it, as if it were
an actor itself.

A Receiver accepts messages sent to it by other entities, counting how many it has
received. It provides methods whereby the thread owning the receiver can query the
count of messages received, mark them as consumed, and block to synchronize with the
arrival of an expected message indicating that some resource or result has become
available.

Receivers also provide the ability to \ref Receiver::RegisterHandler
"register message handlers" to handle the messages they receive, similar to the handler
registration mechanism within actors. This allows the owning thread to register
callback-style handler functions. The registered handlers are executed on the arrival
of messages of the types for which they are registered. The arrived message is passed
to the handler function on execution, allowing it to examine, process, or store its value.

The ability to synchronize with the arrival of messages via \ref Receiver::Wait allows calling
threads to ensure they examine the results of executed handler functions only after
the messages they handle have arrived, and the associated handlers have been executed.
*/
class Receiver : public Detail::Entry::Entity
{
public:

    friend class Framework;

    /**
    \brief Default constructor.

    Constructs a receiver with an automatically-assigned unique address.
    */
    Receiver();

    /**
    \brief Explicit constructor.

    Constructs a receiver tied to the given \ref EndPoint, with the given unique name.

    The optional name parameter allows constructed receiver objects to be given unique,
    user-defined names. These names can then be used to send messages to the receiver in
    situations where the sending code has no direct reference to the destination receiver, so
    can't query it for its address. This is especially important when sending messages
    to remote receiver, and requires that the receiver is tied to  an \ref EndPoint.

    If the name parameter is null then the receiver is given an automatically generated name,
    which is guaranteed to be globally unique as long as the names of all EndPoints are globally
    unique. The generated name can still be used to send messages (if the receiver is tied to
    an EndPoint), but since it is automatically generated it can't be known in advance and
    must be queried by calling \ref GetAddress.

    \code
    class MyActor : public Theron::Actor
    {
    public:

        MyActor(Theron::Framework &framework, const char *const name) :
          Theron::Actor(framework, name)
        {
            RegisterHandler(this, &MyActor::Handler);
        }

    private:

        void Handler(const std::string &message, const Theron::Address from)
        {
            Send(std::string("world"), Theron::Address("receiver"));
        }
    };

    Theron::EndPoint::Parameters params("local", "tcp://192.168.10.104:5555");
    Theron::EndPoint endPoint(params);
    Theron::Framework framework(endPoint);
    Theron::Receiver receiver(endPoint, "receiver");

    MyActor actor(framework, "actor");

    framework.Send(
        std::string("hello"),
        Theron::Address("receiver"),
        Theron::Address("actor"));
    \endcode

    \note The name defined on construction should be globally unique across all connected hosts.
    The name string parameter is copied, so can be destroyed after the call.
    */
    explicit Receiver(EndPoint &endPoint, const char *const name = 0);

    /**
    \brief Destructor.
    */
    ~Receiver();

    /**
    \brief Returns the unique address of the receiver.
    */
    inline Address GetAddress() const;

    /**
    \brief Registers a message handler for a specific message type.

    The handler is a member function on a handler object of type ClassType.
    The handler takes as input a message of type ValueType and a \em from address
    indicating the sender of the message. The handler will be executed whenever
    messages of type ValueType are received.

    The \ref Catcher helper is a useful utility that can be used as a "catcher"
    for messages received by receivers. It's a template and can be instantiated
    against a specific message type, which it then catches. Its \ref Catcher::Push "Push"
    method has the signature of a message handler and so can be registered as a message
    handler using RegisterHandler. It has the semantics of a queue, and acts as
    a FIFO buffer of received messages, ensuring that newly arrived messages don't
    simply overwrite previously received ones. In addition it is thread-safe and
    so can safely be read (popped) while still registered as a handler on a Receiver.

    \code
    struct Message
    {
        int mValue;
    };

    Theron::Receiver receiver;
    Theron::Catcher<Message> catcher;

    // Register the Push method of the Catcher as a handler.
    receiver.RegisterHandler(&catcher, &Theron::Catcher<Message>::Push);
    \endcode

    Having registered the catcher, the calling thread then pops caught messages
    from the Catcher after synchronizing with their arrival via blocking calls to \ref Wait

    \code
    // Wait for two messages to arrive.
    receiver.Wait();
    receiver.Wait();

    // Read the arrived messages from the catcher's internal queue.
    Message message;
    Theron::Address from;

    catcher.Pop(message, from);
    printf("Caught message with value %d\n", message.mValue);

    catcher.Pop(message, from);
    printf("Caught message with value %d\n", message.mValue);
    \endcode

    \tparam ClassType The type of the registered handler class.
    \tparam ValueType The type of the handled messages.
    \param owner Pointer to the registered handler object.
    \param handler Member function pointer to the registered handler function.
    \return True, if the handler was successfully registered.
    */
    template <class ClassType, class ValueType>
    inline bool RegisterHandler(
        ClassType *const owner,
        void (ClassType::*handler)(const ValueType &message, const Address from));

    /**
    \brief Deregisters a previously registered handler.

    \tparam ClassType The type of the registered handler class.
    \tparam ValueType The type of the handled messages.
    \param owner Pointer to the registered handler object.
    \param handler Member function pointer to the registered handler function.
    \return True, if the handler was successfully deregistered.

    \see RegisterHandler
    */
    template <class ClassType, class ValueType>
    inline bool DeregisterHandler(
        ClassType *const owner,
        void (ClassType::*handler)(const ValueType &message, const Address from));

    /**
    \brief Resets to zero the count of messages received but not yet consumed.

    The internal count of the number of messages that have arrived but not yet
    been consumed (by calls to \ref Wait or \ref Consume) is reset to zero,
    effectively forgetting about all messages that have previously arrived.

    \see Count
    */
    inline void Reset();

    /**
    \brief Returns the number of messages received but not yet consumed.

    The \ref Count method allows a calling thread to query the Receiver's count
    of arrived but unconsumed messages. The count is returned without blocking the
    calling thread or waiting for any messages to be received.

    Each Receiver maintains an internal count of the number of messages it
    has received which have not yet been accounted for by calls to \ref Wait
    or \ref Consume. Effectively this is the number of arrived messages of
    which the caller is not yet aware or has not yet synchronized.

    By periodically calling \ref Count, a thread can check for arrived messages
    while also continuing with its own work. If the returned count indicates that
    the receiver contains newly arrived, unconsumed messages, then the calling
    thread can call \ref Consume to mark one or more messages as consumed,
    subtracting them from the count.

    \see Reset
    \see Consume
    */
    inline uint32_t Count() const;

    /**
    \brief Waits until one or more messages arrive at the receiver.

    The Wait method allows a calling thread to wait for, and synchronize with, the arrival
    of an expected message. The \ref Wait method takes an optional parameter specifying
    the maximum number of messages expected, which defaults to one. On calling \ref Wait,
    the calling thread blocks if no messages are available that have not already been consumed,
    or accounted for, by previous calls to Wait or \ref Consume.

    The Receiver maintains an internal count of how many messages have been received but not
    yet consumed, and a call to \ref Wait will return immediately if the call can be
    satisfied by an unconsumed message that has already arrived. This means that a caller
    wishing to synchronize with the arrival of an expected message can safely call \ref Wait,
    irrespective of whether some or all of a number of expected messages may have already
    arrived. If one or more messages have arrived the call simply returns immediately without
    blocking, consuming all arrived messages up to the specified maximum. Otherwise
    it blocks until a message arrives, whereupon the thread is woken and returns.
    
    When a received message becomes available, it is \em consumed and will not be used to
    satisfy any later calls to \ref Wait.
    
    If a maximum count was specified as an argument to \ref Wait then all available messages
    are consumed, up to the specified limit. If no limit is specified then just one message is
    consumed. In either case Wait returns the number of messages consumed, allowing the caller
    to update its own accounting of messages expected and received. By supplying a maximum
    limit greater than one, the caller expecting many messages can consume multiple
    messages in one call, without having to call Wait() for each one in turn.

    \code
    // Wait for the arrival of 10 expected messages, blocking until a message arrives
    // and then consuming all arrived messages in one call before waiting again.
    int outstandingCount(10);
    while (outstandingCount)
    {
        outstandingCount -= receiver.Wait(outstandingCount);
    }
    \endcode

    \note This is a blocking call and, if no message is already available, it will cause
    the calling thread to block indefinitely until a message is received.

    \param max Maximum number of arrived messages to be consumed on this call.
    \return The actual number of arrived messages consumed on this call.
    */
    inline uint32_t Wait(const uint32_t max = 1);

    /**
    \brief Consumes any unconsumed messages available on the receiver, up to a specified limit.

    The \ref Consume method consumes any messages that have arrived at the receiver but
    not yet been \em consumed by previous calls to \ref Wait or \ref Consume, up to the
    a specified maximum number. The calling thread returns immediately, without waiting
    for any more messages to arrive, and will return without consuming any messages if none
    are available at the time of the call.

    In conjunction with \ref Count, the \ref Consume method provides a way to consume one
    or more expected messages that have arrived, in a single call without waiting. \ref Consume
    returns the actual number of messages consumed, allowing the caller to update their own
    accounting of arrived messages. The effect of consuming the messages is to subtract them
    from the internal count of unconsumed messages, making them unavailable to further calls
    of \ref Wait and \ref Consume. The internal count can be queried by \ref Count.

    \code
    // Wait for the arrival of 10 expected messages while also doing other work.
    // Repeatedly call Consume to consume any arrived messages without blocking.
    int outstandingCount(10);
    while (outstandingCount)
    {
        outstandingCount -= receiver.Consume(outstandingCount);

        // Do some other work before checking again
        // ...

    }
    \endcode

    \return The number of messages actually consumed, which may be zero.
    */
    inline uint32_t Consume(const uint32_t max);

private:

    typedef Detail::List<Detail::IReceiverHandler> MessageHandlerList;

    Receiver(const Receiver &other);
    Receiver &operator=(const Receiver &other);

    /**
    Initializes a receiver.
    This function is called by the various constructor flavors and avoids repeating the code.
    */
    void Initialize();

    /**
    Tears down a receiver prior to destruction.
    */
    void Release();

    /**
    \brief Pushes a message into the receiver.
    */
    inline void Push(Detail::IMessage *const message);

    Detail::StringPool::Ref mStringPoolRef;             ///< Ensures that the StringPool is created.
    EndPoint *const mEndPoint;                          ///< Pointer to the network endpoint, if any, to which this receiver is tied.
    Detail::String mName;                               ///< Name of the receiver.
    Address mAddress;                                   ///< Unique address of this receiver.
    MessageHandlerList mMessageHandlers;                ///< List of registered message handlers.
    mutable Detail::Condition mCondition;               ///< Signals waiting threads when messages arrive.
    mutable Detail::Atomic::UInt32 mMessagesReceived;   ///< Counts arrived messages not yet waited on.
};


// TODO: Force-inline
inline Address Receiver::GetAddress() const
{
    return mAddress;
}


template <class ClassType, class ValueType>
inline bool Receiver::RegisterHandler(
    ClassType *const owner,
    void (ClassType::*handler)(const ValueType &message, const Address from))
{
    // When a receiver registers a handler for a message type, we get told about
    // the type. So we take the opportunity to also register the type against
    // its name with the network endpoint (if the receiver is tied to one). This enables
    // us to recognize the type when it arrives in a network message as a block
    // blind of data tagged with a type name.
    if (mEndPoint)
    {
        mEndPoint->RegisterMessageType<ValueType>();
    }

    // If the message value type has a valid (non-zero) type name defined for it,
    // then we use explicit type names to match messages to handlers.
    // The default value of zero will indicates that no type name has been defined,
    // in which case we rely on automatically stored RTTI to identify message types.
    typedef Detail::ReceiverHandler<ClassType, ValueType> MessageHandlerType;

    // Allocate memory for a message handler object.
    void *const memory = AllocatorManager::GetCache()->Allocate(sizeof(MessageHandlerType));
    if (memory == 0)
    {
        return false;
    }

    // Construct a handler object to remember the function pointer and message value type.
    MessageHandlerType *const messageHandler = new (memory) MessageHandlerType(owner, handler);
    if (messageHandler == 0)
    {
        return false;
    }

    mCondition.GetMutex().Lock();
    mMessageHandlers.Insert(messageHandler);
    mCondition.GetMutex().Unlock();
    
    return true;
}


template <class ClassType, class ValueType>
inline bool Receiver::DeregisterHandler(
    ClassType *const /*owner*/,
    void (ClassType::*handler)(const ValueType &message, const Address from))
{
    // If the message value type has a valid (non-zero) type name defined for it,
    // then we use explicit type IDs to match messages to handlers.
    // The default value of zero will indicate that no type name has been defined,
    // in which case we rely on automatically stored RTTI to identify message types.
    typedef Detail::ReceiverHandler<ClassType, ValueType> MessageHandlerType;
    typedef Detail::ReceiverHandlerCast<ClassType, Detail::MessageTraits<ValueType>::HAS_TYPE_NAME> HandlerCaster;

    mCondition.GetMutex().Lock();

    // Find the handler in the registered handler list.
    typename MessageHandlerList::Iterator handlers(mMessageHandlers.GetIterator());
    while (handlers.Next())
    {
        Detail::IReceiverHandler *const messageHandler(handlers.Get());

        // Try to convert this handler, of unknown type, to the target type.
        const MessageHandlerType *const typedHandler = HandlerCaster:: template CastHandler<ValueType>(messageHandler);
        if (typedHandler)
        {
            if (typedHandler->GetHandlerFunction() == handler)
            {
                // Remove the handler from the list.
                mMessageHandlers.Remove(messageHandler);

                // Free the handler object, which was allocated on registration.
                AllocatorManager::GetCache()->Free(messageHandler, sizeof(MessageHandlerType));

                break;
            }
        }
    }

    mCondition.GetMutex().Unlock();

    return false;
}


THERON_FORCEINLINE void Receiver::Reset()
{
    Detail::Lock lock(mCondition.GetMutex());
    mMessagesReceived.Store(0);
}


THERON_FORCEINLINE uint32_t Receiver::Count() const
{
    Detail::Lock lock(mCondition.GetMutex());
    return static_cast<uint32_t>(mMessagesReceived.Load());
}


THERON_FORCEINLINE uint32_t Receiver::Wait(const uint32_t max)
{
    THERON_ASSERT(max > 0);

    Detail::Lock lock(mCondition.GetMutex());

    // Wait for at least one message to arrive.
    // If messages were received since the last wait (or creation),
    // then we regard those messages as qualifying and early-exit.
    while (mMessagesReceived.Load() == 0)
    {
        // Wait to be woken by an arriving message.
        // This blocks until a message arrives!
        mCondition.Wait(lock);
    }

    uint32_t numConsumed(0);
    while (mMessagesReceived.Load() > 0 && numConsumed < max)
    {
        mMessagesReceived.Decrement();
        ++numConsumed;
    }

    return numConsumed;
}


THERON_FORCEINLINE uint32_t Receiver::Consume(const uint32_t max)
{
    // Consume up to the maximum number of arrived messages.
    THERON_ASSERT(max > 0);

    Detail::Lock lock(mCondition.GetMutex());

    uint32_t numConsumed(0);
    while (mMessagesReceived.Load() > 0 && numConsumed < max)
    {
        mMessagesReceived.Decrement();
        ++numConsumed;
    }

    return numConsumed;
}


THERON_FORCEINLINE void Receiver::Push(Detail::IMessage *const message)
{
    THERON_ASSERT(message);

    mCondition.GetMutex().Lock();

    // TODO: Use ReceiverHandlerCollection for thread-safety.
    MessageHandlerList::Iterator handlers(mMessageHandlers.GetIterator());
    while (handlers.Next())
    {
        // Execute the handler.
        // It does nothing if it can't handle the message type.
        Detail::IReceiverHandler *const handler(handlers.Get());
        handler->Handle(message);
    }

    mMessagesReceived.Increment();

    mCondition.GetMutex().Unlock();
    mCondition.PulseAll();

    // Destroy the message.
    // We use the global allocator to allocate messages sent to receivers.
    IAllocator *const messageAllocator(AllocatorManager::GetCache());
    Detail::MessageCreator::Destroy(messageAllocator, message);
}


} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_RECEIVER_H
