// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_FRAMEWORK_H
#define THERON_FRAMEWORK_H


/**
\file Framework.h
Framework that hosts and executes actors.
*/


#include <new>

#include <Theron/Address.h>
#include <Theron/Align.h>
#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>
#include <Theron/YieldStrategy.h>

#include <Theron/Detail/Allocators/CachingAllocator.h>
#include <Theron/Detail/Debug/BuildDescriptor.h>
#include <Theron/Detail/Directory/Directory.h>
#include <Theron/Detail/Directory/Entry.h>
#include <Theron/Detail/Handlers/DefaultFallbackHandler.h>
#include <Theron/Detail/Handlers/FallbackHandlerCollection.h>
#include <Theron/Detail/Mailboxes/Mailbox.h>
#include <Theron/Detail/Messages/MessageCreator.h>
#include <Theron/Detail/Scheduler/Counting.h>
#include <Theron/Detail/Scheduler/MailboxContext.h>
#include <Theron/Detail/Scheduler/IScheduler.h>
#include <Theron/Detail/Strings/String.h>
#include <Theron/Detail/Strings/StringPool.h>
#include <Theron/Detail/Threading/Atomic.h>
#include <Theron/Detail/Threading/SpinLock.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#pragma warning (disable:4996)  // function or variable may be unsafe
#endif //_MSC_VER


namespace Theron
{


class Actor;
class EndPoint;


/**
\brief Manager class that hosts, manages, and executes actors.

Users should construct an instance of the Framework in non-actor application
code before creating any actors. Actor objects can then be created by passing
the framework as a parameter to the constructor of the derived actor class:

\code
class MyActor : public Theron::Actor
{
public:

    explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
    {
    }
};

Theron::Framework framework;
MyActor myActor(framework);
\endcode

Internally, each framework contains a pool of worker threads which are used
to execute the message handlers of the actors created within it. The threads
within a framework are dedicated to executing actors within that framework,
and are never used to execute actors in other frameworks or any other user code.

The initial number of worker threads can be specified on construction of the
framework by means of an explicit parameter to the Framework::Framework constructor.
Additionally, the number of threads can be increased or decreased at runtime
by calling \ref SetMinThreads or \ref SetMaxThreads. The utilization of the currently
enabled threads is measured by performance metrics which can be queried with
\ref GetCounterValue.

The worker threads are created and synchronized using underlying threading
objects. Different implementations of these threading objects are possible,
allowing Theron to be used in environments with different threading primitives.
Currently, implementations based on Win32 threads, Boost threads, std::thread
and POSIX threads are provided. Users can define \ref THERON_BOOST or
\ref THERON_CPP11 to enable or disable the use of boost::thread and std::thread,
respectively.

It's possible to construct multiple Framework objects within a single
application. The actors created within each Framework are processed
by separate pools of worker threads. Actor addresses are globally unique
across all frameworks. Actors in one framework can send messages to actors
in other frameworks.

Actors created within each Framework are executed only by the worker threads
in the threadpool of that framework, allowing the threads of a Framework to
be effectively dedicated to particular actors. See
\ref Theron::Framework::Parameters for more information.

\note An important point about Framework objects is that they must always
outlive the actors created within them.
*/
class Framework : public Detail::Entry::Entity
{
public:

    friend class Actor;
    friend class EndPoint;

    /**
    \brief Parameters structure that can be passed to the Framework constructor.
    
    When an instance of this structure is passed to the constructor of the Framework class,
    its members can be used to configure the constructed framework.

    The members are mainly concerned with configuring the pool of worker threads that is created
    within the framework, and which is used to execute the actors that are subsequently hosted
    within it. They allow the user to control the number of threads in the pool, their processor
    affinities, and their yielding behavior.
    
    By choosing appropriate values it's possible to configure specialized frameworks tailored to
    particular uses. For example, users writing real-time systems might choose to create a separate
    framework for hosting actors whose execution is time-critical. Such a framework might be configured
    with the same number of threads as the number of hosted actors, ensuring that a free thread is
    always available to process actors with queued messages. Also, one might choose to dedicate a
    number of processor cores entirely to the execution of those threads, using processor
    affinity masks to limit execution of the worker threads of the framework to only the selected
    cores (leaving the remaining cores to other frameworks). Finally, since the cores dedicated to
    time-critical processing are never used for any other processing, one might choose to set the yield
    strategy of the worker threads to \ref YIELD_STRATEGY_SPIN, effectively busy-waiting for
    the arrival of new messages.

    As well as setting processor affinities, the members of the Parameters structure allow the
    node affinities of the worker threads to be controlled. This allows users to restrict
    the execution of a framework's worker threads to a particular \em node within a NUMA
    (Non-Uniform Memory Architecture) system.

    A NUMA system is one in which the different logical cores within the physical CPU(s) have
    different views of memory, typically by virtue of being serviced by different physical memory
    controllers. In such systems, memory may be partitioned into areas that are each directly
    accessed by a single memory controller, with the cores fed by that controller enjoying faster
    access to that area of memory. Access to other parts of memory is indirectly served by other
    controllers, so is slower. Within the context of a NUMA system, a 'node' is a group of logical
    cores that share the same view of memory (typically by virtue of being serviced by the same
    memory controller).
    
    Windows and Linux both provide APIs by which the node topology of a system can be discovered. These
    APIs allow the processor affinities of threads to be set on a per-node basis, limiting threads
    to execute only on the processors of a particular nodes (or set of nodes). Finally, they provide
    methods for allocating memory within the area of memory to which a particular node has first class
    access.

    The scalability of a multi-threaded application can be improved by restricting threads that access
    the same memory to execute only on a limited subset of the available cores. Doing so can improve
    cache coherency due to the same memory being repeatedly accessed via the caches of those cores.

    As threads read and write memory, cached copies of the memory accumulate in the caches local
    to the cores on which the threads are executed. When a thread writes to memory, any cached copies held
    in the caches of other cores are invalidated and must be re-fetched from memory. If other threads
    also write to the same piece of memory (known as \em shared \em writes), then the repeated cache
    invalidations and refreshes can cause significant overheads. These overheads can be especially
    severe, and can limit scalability, if the threads in question are allowed to execute on different
    NUMA nodes.

    In Theron, Frameworks serve as the mechanism by which worker threads are grouped. Accordingly,
    Theron allows the node and processor affinities of worker threads to be set on a per-framework
    basis. The expectation is that the actors within a single framework will mainly message
    each other, with messages being sent between frameworks far less frequently.

    \note Support for node and processor affinity masks is currently somewhat limited.
    Supported is implemented with Windows NUMA API in windows builds, and with libnuma under linux.
    In GCC builds, NUMA support requires libnuma-dev and must be explicitly enabled via \ref THERON_NUMA
    (or numa=on in the makefile). The \ref mNodeMask member is supported with both version 1 and version 2
    of the libnuma API, but the \ref mProcessorMask member is supported only with version 2. Under Windows
    both members are supported.
    */
    struct Parameters
    {
        /**
        \brief Constructor.
        Constructs a parameters object with a specified initial thread count, running on a specified
        subset of the processors of each of a specified set of NUMA processor nodes.

        \param threadCount Number of worker threads to create initially within the framework.
        \param nodeMask Bitfield mask specifying the NUMA node affinity of the created worker threads.
        \param processorMask Bitfield mask specifying the processor affinity of the created worker threads within each enabled NUMA node.
        \param yieldStrategy Enum value specifying how freely worker threads yield to other system threads.
        \param priority Relative scheduling priority of the worker threads (range -1.0 to 1.0, 0.0 means "normal").
        */
        inline explicit Parameters(
            const uint32_t threadCount = 16,
            const uint32_t nodeMask = 0x1,
            const uint32_t processorMask = 0xFFFFFFFF,
            const YieldStrategy yieldStrategy = YIELD_STRATEGY_CONDITION,
            const float priority = 0.0f) :
          mThreadCount(threadCount),
          mNodeMask(nodeMask),
          mProcessorMask(processorMask),
          mYieldStrategy(yieldStrategy),
          mThreadPriority(priority)
        {
        }

        uint32_t mThreadCount;          ///< The initial number of worker threads to create within the framework.
        uint32_t mNodeMask;             ///< 32-bit mask specifying the NUMA processor nodes upon which the framework may execute.
        uint32_t mProcessorMask;        ///< 32-bit mask specifying the subset of the processors in each NUMA processor node upon which the framework may execute.
        YieldStrategy mYieldStrategy;   ///< Member of \ref YieldStrategy specifying how worker threads yield to other system threads when no work is available.
        float mThreadPriority;          ///< Number between -1.0 and 1.0 indicating the relative scheduling priority of the worker threads.
    };

    /**
    \brief Constructor.

    Constructs a framework with the given number of worker threads.

    \note This constructor is deprecated. Use the \ref Parameters structure instead.

    \code
    class MyActor : public Theron::Actor
    {
    public:

        explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework frameworkOne(8);
    MyActor actorOne(frameworkOne);

    Theron::Framework frameworkTwo(12);
    MyActor actorTwo(frameworkTwo);
    \endcode
    */
    inline explicit Framework(const uint32_t threadCount);

    /**
    \brief Constructor.

    Constructs a framework with the given parameters. The parameters
    control the size and processor affinity of a framework's threadpool.

    \code
    class MyActor : public Theron::Actor
    {
    public:

        explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    const Theron::Framework::Parameters params(8);
    Theron::Framework framework(params);

    MyActor actorOne(framework);
    MyActor actorTwo(framework);
    \endcode

    \note In distributed applications, use the constructor variant that accepts an \ref EndPoint.
    */
    inline explicit Framework(const Parameters &params = Parameters());

    /**
    \brief Constructor.

    Constructs a framework object with the given parameters, tied to the given network endpoint.
    A framework that is tied to a network endpoint is able to send messages to, and receive
    messages from, remote actors hosted in frameworks tied to other endpoints.

    \code
    class MyActor : public Theron::Actor
    {
    public:

        explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    const Theron::EndPoint::Parameters params("endpoint_one", "tcp://192.168.10.104:5555");
    Theron::EndPoint endPoint(params);

    Theron::Framework framework(endPoint, "framework_one");
    MyActor actorOne(framework);
    \endcode

    \param endPoint The endpoint to which the framework instance is tied.
    \param name An optional user-defined name for the framework, which must be unique within the context of the endpoint.
    \param params Optional parameters object providing construction parameters.

    \note The name string parameter is copied, so can be destroyed after the call.
    */
    inline Framework(EndPoint &endPoint, const char *const name = 0, const Parameters &params = Parameters());

    /**
    \brief Destructor.

    Destroys a Framework.

    A Framework must always outlive (ie. be destructed after) any actors created within
    it. Specifically, all actors created within a Framework must have been destructed
    themselves before the Framework itself is allowed to be destructed.

    \code
    class Actor : public Theron::Actor
    {
    public:

        explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    main()
    {
        Theron::Framework framework;
        Actor *actor = new Actor(framework);

        // Must delete (ie. destruct) the actor before the Framework hosting it!
        delete actor;
    }
    \endcode

    \note If a Framework instance is allowed to be destructed before actors created within
    it, the actors will not be correctly destroyed, leading to errors or memory leaks.
    */
    inline ~Framework();

    /**
    \brief Sends a message to the entity (typically an actor, but potentially a Receiver) at the given address.

    \code
    class Actor : public Theron::Actor
    {
    public:

        explicit MyActor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Theron::Receiver receiver;
    Actor actor(framework);

    framework.Send(std::string("Hello"), receiver.GetAddress(), actor.GetAddress());
    \endcode

    If no actor or receiver exists with the given address, the message will not be
    delivered, and Send will return false.

    This can happen if the target entity is an actor but has been destructed due to
    going out of scope (if it was constructed as a local on the stack) or explicitly
    deleted (if it was constructed on the heap via operator new).

    If the destination address exists, but the receiving entity has no handler
    registered for messages of the sent type, then the message will be ignored,
    but this method will still return true. Note that a true return value doesn't
    necessarily imply that the message was actually handled by the actor.

    In either event, if the message is not actually handled by an actor, the default
    \ref Framework::SetFallbackHandler "fallback handler" will assert to alert the
    user about the unhandled message.

    \note This method is used mainly to send messages from non-actor code, eg. main(),
    where only a Framework instance is available. In such cases, the address of a receiver
    is typically passed as the 'from' address. When sending messages from within an actor,
    it is more natural to use \ref Actor::Send, where the address of the sending actor is
    implicit.

    \tparam ValueType The message type.
    \param value The message value.
    \param from The address of the sending entity (typically a receiver).
    \param address The address of the target entity (an actor or a receiver).
    \return True, if the message was delivered to an entity, otherwise false.
    */
    template <typename ValueType>
    inline bool Send(const ValueType &value, const Address &from, const Address &address);

    /**
    \brief Specifies a maximum limit on the number of worker threads enabled in this framework.

    This method allows an application to place an upper bound on the thread count.
    Users can use this method, together with SetMinThreads, to implement a policy for
    framework threadpool management.

    This method will only decrease the actual number of worker threads, never increase it.
    Calling this method is guaranteed to eventually result in the number of threads being
    less than or equal to the specified limit, as long as messages continue to be sent and
    unless a higher minimum limit is specified subsequently.

    If two successive calls specify different maximums, the lower takes effect. If
    conflicting minimum and maximums are specified by subsequent calls to this method
    and SetMinThreads, then the later call wins.

    The idea behind separate minimum and maximum limits, rather than a single method to
    directly set the actual number of threads, is to allow negotiation between multiple
    agents, each with a different interest in the thread count. One may require a certain
    minimum number of threads for its processing, but not care if the actual number of
    threads is higher, while another may wish to impose a maximum limit on the number of
    threads in existence, but be satisfied if there are less.

    \note If the number of threads before the call was higher than the requested maximum,
    there may be an arbitrary delay after calling this method before the number of threads
    drops to the requested value. The number of threads is managed over time, and is not
    guaranteed to be less than or equal to the requested maximum immediately after the call.
    Threads self-terminate on being woken, if they discover that the actual thread count
    is higher than the limit. This means that until some threads are woken by the arrival
    of new messages, the actual thread count will remain unchanged.

    \param count A positive integer - behavior for zero is undefined.

    \see SetMinThreads
    */
    inline void SetMaxThreads(const uint32_t count);

    /**
    \brief Specifies a minimum limit on the number of worker threads enabled in this framework.

    This method allows an application to place a lower bound on the thread count.
    Users can use this method, together with SetMaxThreads, to implement a policy for
    framework threadpool management.

    This method will only increase the actual number of worker threads, never reduce it.
    Calling this method is guaranteed to eventually result in the number of threads being
    greater than or equal to the specified limit, unless a lower maximum limit is specified
    subsequently.

    If two successive calls specify different minimums, the higher takes effect. If
    conflicting minimum and maximums are specified by subsequent calls to this method
    and SetMaxThreads, then the later call wins.

    \note If the number of threads before the call was lower than the requested minimum,
    there may be an arbitrary delay after calling this method before the number of threads rises
    to the requested value. Threads are spawned or re-enabled by a manager thread dedicated
    to that task, which runs asynchronously from other threads as a background task.
    It spends most of its time asleep, only being woken by calls to SetMinThreads.

    \param count A positive integer - behavior for zero is undefined.

    \see SetMaxThreads
    */
    inline void SetMinThreads(const uint32_t count);

    /**
    \brief Returns the current maximum limit on the number of worker threads in this framework.

    This method returns the current maximum limit on the size of the worker threadpool.
    Setting a maximum thread limit with SetMaxThreads doesn't imply that that limit will be
    returned by this function. The target thread count is negotiated over multiple calls,
    and specifying a higher value than the current maximum may have no effect.

    \note In the current implementation, GetMaxThreads and GetMinThreads return the same
    value, which is the current target thread count. Note that this may be different from
    the actual current number of threads, returned by GetNumThreads.

    \see GetMinThreads
    */
    inline uint32_t GetMaxThreads() const;

    /**
    \brief Returns the current minimum limit on the number of worker threads in this framework.

    This method returns the current minimum limit on the size of the worker threadpool.
    Setting a minimum thread limit with SetMinThreads doesn't imply that that limit will be
    returned by this function. The target thread count is negotiated over multiple calls,
    and specifying a lower value than the current minimum may have no effect.

    \see GetMaxThreads
    */
    inline uint32_t GetMinThreads() const;

    /**
    \brief Gets the actual number of worker threads currently in this framework.

    The returned count reflects the actual number of enabled threads at the time of the
    call, which is independent from any maximum or minimum limits specified with
    SetMaxThreads or SetMinThreads. The count includes all enabled threads, including any
    that are sleeping due to having no work to do, but not including ones which were
    created earlier but subsequently terminated to reduce the thread count.

    \note The value returned by this method is specific to this framework instance. If
    multiple frameworks are created then each has its own threadpool with an independently
    managed thread count.

    \see GetPeakThreads
    */
    inline uint32_t GetNumThreads() const;

    /**
    \brief Gets the peak number of worker threads ever active in the framework.

    This call queries the highest number of simultaneously enabled threads seen since the
    start of the framework. Note that this measures the highest actual number of threads,
    as measured by GetNumThreads, rather than the highest values of the maximum or minimum
    thread count limits.

    \note The value returned by this method is specific to this framework instance. If
    multiple frameworks are created then each has its own threadpool with an independently
    managed thread count.
    */
    inline uint32_t GetPeakThreads() const;

    /**
    \brief Returns the number of counters available for querying via GetCounterValue.

    Each Framework maintains a number of performance counters that count performance
    events. Many of these counters are really for internal development use and are
    not intended to be meaningful to users.

    Counters are indexed consecutively from zero, so the counter with highest index
    has index equal to one less than the returned value. If the returned value is zero
    then no counters are available.

    \note Counters are only available if \ref THERON_ENABLE_COUNTERS is defined as non-zero.

    \see GetCounterName
    */
    inline uint32_t GetNumCounters() const;

    /**
    \brief Returns the string name of the counter with the given index.
    \see GetNumCounters
    */
    inline const char *GetCounterName(const uint32_t counter) const;

    /**
    \brief Resets all internal event counters for this framework.

    \see GetCounterValue
    */
    inline void ResetCounters();

    /**
    \brief Gets the current value of a specified event counter.

    Each Framework maintains a set of internal event counters.
    This method gets the current value of a specific counter, aggregated over all worker threads.

    \note Counters are only available if \ref THERON_ENABLE_COUNTERS is defined as non-zero.

    \param counter An integer index identifying the counter to be queried.
    \return Current value of the counter at the time of the call.

    \see GetNumCounters
    \see GetPerThreadCounterValues
    \see ResetCounters
    */
    inline uint32_t GetCounterValue(const uint32_t counter) const;

    /**
    \brief Gets the current per-thread values of a specified event counter.

    Each Framework maintains a set of internal event counters.
    This method gets the current value of a specific counter for each of the currently active worker threads.

    \note Counters are only incremented if \ref THERON_ENABLE_COUNTERS is defined as non-zero.

    \param counter An integer index identifying the counter to be queried.
    \param perThreadCounts Pointer to an array of uint32_t to be filled with per-thread counter values.
    \param maxCounts The size of the perThreadCounts array and hence the maximum number of values to fetch.
    \return The actual number of per-thread values fetched, matching the number of currently active worker threads.

    \see GetCounterValue
    \see ResetCounters
    */
    inline uint32_t GetPerThreadCounterValues(
        const uint32_t counter,
        uint32_t *const perThreadCounts,
        const uint32_t maxCounts) const;

    /**
    \brief Sets the fallback message handler executed for unhandled messages.

    The fallback handler registered with the framework is run:
    - when a message is sent by an actor within this framework to an address at which no entity is currently registered.
    - when a message is sent using the \ref Send method of this framework to an address at which no entity is currently registered.
    - when a message is delivered to an actor within this framework in which neither a handler for that message type nor a default handler are registered, with the result that the message goes unhandled.

    The main purpose of the fallback handler is for error reporting, and the default
    fallback handler, which is used if no user-defined handler is explicitly set,
    asserts to indicate an unhandled message, which is a common programming error.
    The reporting of unhandled messages by the default fallback handler can be controlled
    by the \ref THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS define, whose default value is
    defined in \ref Defines.h.

    The handler function registered by this method overload is represented by a pointer to
    a user-defined handler object, and a member function pointer identifying a handler
    function that is a member function of the handler object. Handlers registered using
    this method must expose a handler method that accepts a 'from' address, as shown by
    the following example:

    \code
    class Handler
    {
    public:

        inline void Handle(const Theron::Address from)
        {
            printf("Caught undelivered or unhandled message sent from address '%d'\n", from.AsInteger());
        }
    };

    Theron::Framework framework;
    Handler handler;
    framework.SetFallbackHandler(&handler, &Handler::Handle);
    \endcode

    Users can call this method to replace the default fallback handler with their own
    implementation, for example for more sophisticated error reporting or logging.

    Passing 0 to this method clears any previously set fallback handler, or the
    default fallback handler if no user-defined handler has previously been set.

    \note There are two variants of SetFallbackHandler, accepting fallback handlers
    with different function signatures. Handlers set using this method accept only a
    'from' address, whereas handlers set using the other method also accept the 
    unhandled message as 'blind' data (ie. a void pointer and a size in bytes).
    Registering either kind of fallback handler replaces any previously set handler
    of the other kind.

    \tparam ObjectType The type of the handler object which owns the handler function.
    \param actor Pointer to the handler object on which the handler function is a member function.
    \param handler Member function pointer identifying the fallback handler function.
    */
    template <typename ObjectType>
    inline bool SetFallbackHandler(
        ObjectType *const actor,
        void (ObjectType::*handler)(const Address from));

    /**
    \brief Sets the fallback message handler executed for unhandled messages.

    This method sets a 'blind' default handler which, when executed, is passed the
    unknown message as blind data. The blind data consists of the memory block containing
    the message, identified by a void pointer and a size.

    The fallback handler registered with the framework is run:
    - when a message is sent by an actor within this framework to an address at which no entity is currently registered.
    - when a message is sent using the \ref Send method of this framework to an address at which no entity is currently registered.
    - when a message is delivered to an actor within this framework in which neither a handler for that message type nor a default handler are registered, with the result that the message goes unhandled.

    The handler function registered by this method overload is passed the unhandled message
    as 'blind' data represented by a void pointer and a message size in bytes. Handlers
    registered using this method must expose a handler method that accepts a void pointer
    identifying the message contents, a size indicating the size of the message data, and a
    'from' address.

    A user-defined handler may be able to inspect the contents of the message and
    take appropriate action, such as reporting the contents of an unexpected message to help
    with debugging.

    \code
    class Handler
    {
    public:

        inline void Handle(const void *const data, const Theron::uint32_t size, const Theron::Address from)
        {
            printf("Caught undelivered or unhandled message of size %d sent from address '%d'\n", size, from.AsInteger());
        }
    };

    Theron::Framework framework;
    Handler handler;
    framework.SetFallbackHandler(&handler, &Handler::Handle);
    \endcode

    Users can call this method to replace the default fallback handler with their own
    implementation, for example for more sophisticated error reporting or logging.

    Passing 0 to this method clears any previously set fallback handler, or the
    default fallback handler if no user-defined handler has previously been set.

    \note There are two variants of SetFallbackHandler, accepting fallback handlers
    with different function signatures. Handlers set using this method accept the 
    unhandled message as 'blind' data (ie. a void pointer and a size in bytes),
    whereas handlers set using the other method accept only the 'from' address.
    Registering either kind of fallback handler replaces any previously set handler
    of the other kind.

    \tparam ObjectType The type of the handler object which owns the handler function.
    \param actor Pointer to the handler object on which the handler function is a member function.
    \param handler Member function pointer identifying the fallback handler function.
    */
    template <typename ObjectType>
    inline bool SetFallbackHandler(
        ObjectType *const actor,
        void (ObjectType::*handler)(const void *const data, const uint32_t size, const Address from));

private:

    struct MessageCacheTraits
    {
        typedef Detail::SpinLock LockType;

        struct THERON_PREALIGN(THERON_CACHELINE_ALIGNMENT) AlignType
        {
        } THERON_POSTALIGN(THERON_CACHELINE_ALIGNMENT);

        static const uint32_t MAX_POOLS = 8;
        static const uint32_t MAX_BLOCKS = 16;
    };

    typedef Detail::CachingAllocator<MessageCacheTraits> MessageCache;

    Framework(const Framework &other);
    Framework &operator=(const Framework &other);

    /**
    Initializes a framework object at start of day.
    This function is called by the various constructor flavors and avoids repeating the code.
    */
    void Initialize();

    /**
    Tears down a framework object prior to destruction.
    */
    void Release();

    /**
    Allocates and initializes an owned scheduler object.
    */
    Detail::IScheduler *CreateScheduler();

    /**
    Destroys a previously created scheduler object.
    */
    void DestroyScheduler(Detail::IScheduler *const scheduler);

    /**
    Registers a new actor in the directory and allocates a mailbox.
    */
    void RegisterActor(Actor *const actor, const char *const name = 0);

    /**
    Deregisters a previously registered actor.
    */
    void DeregisterActor(Actor *const actor);

    /**
    Helper method that sends messages.
    */
    inline bool SendInternal(
        Detail::MailboxContext *const mailboxContext,
        Detail::IMessage *const message,
        Address address);

    /**
    Helper method that sends messages to entities in the local process.
    */
    static bool DeliverWithinLocalProcess(
        Detail::IMessage *const message,
        const Detail::Index &index);

    /**
    Receives a message from another framework.
    */
    inline bool FrameworkReceive(
        Detail::IMessage *const message,
        const Address &address);

    /**
    Returns a pointer to the shared mailbox context not associated with a specific worker thread.
    */
    inline Detail::MailboxContext *GetMailboxContext();

    Detail::StringPool::Ref mStringPoolRef;                 ///< Ensures that the StringPool is created.
    EndPoint *const mEndPoint;                              ///< Pointer to the network endpoint, if any, to which this framework is tied.
    const Parameters mParams;                               ///< Copy of parameters struct provided on construction.
    uint32_t mIndex;                                        ///< Non-zero index of this framework, unique within the local process.
    Detail::String mName;                                   ///< Name of this framework.
    Detail::Directory<Detail::Mailbox> mMailboxes;          ///< Per-framework mailbox array.
    Detail::FallbackHandlerCollection mFallbackHandlers;    ///< Registered message handlers run for unhandled messages.
    Detail::DefaultFallbackHandler mDefaultFallbackHandler; ///< Default handler for unhandled messages.
    MessageCache mMessageAllocator;                         ///< Thread-safe per-framework cache of message memory blocks.
    Detail::MailboxContext mSharedMailboxContext;           ///< Shared per-framework mailbox context.
    Detail::IScheduler *mScheduler;                         ///< Pointer to owned scheduler implementation.
};


inline Framework::Framework(const uint32_t threadCount) :
  mStringPoolRef(),
  mEndPoint(0),
  mParams(threadCount),
  mIndex(0),
  mName(),
  mMailboxes(),
  mFallbackHandlers(),
  mDefaultFallbackHandler(),
  mMessageAllocator(AllocatorManager::GetCache()),
  mSharedMailboxContext(),
  mScheduler(0)
{
    Detail::BuildDescriptor::Check();

    Initialize();
}


inline Framework::Framework(const Parameters &params) :
  mStringPoolRef(),
  mEndPoint(0),
  mParams(params),
  mIndex(0),
  mName(),
  mMailboxes(),
  mFallbackHandlers(),
  mDefaultFallbackHandler(),
  mMessageAllocator(AllocatorManager::GetCache()),
  mSharedMailboxContext(),
  mScheduler(0)
{
    Detail::BuildDescriptor::Check();

    Initialize();
}


inline Framework::Framework(EndPoint &endPoint, const char *const name, const Parameters &params) :
  mStringPoolRef(),
  mEndPoint(&endPoint),
  mParams(params),
  mIndex(0),
  mName(name),
  mMailboxes(),
  mFallbackHandlers(),
  mDefaultFallbackHandler(),
  mMessageAllocator(AllocatorManager::GetCache()),
  mSharedMailboxContext(),
  mScheduler(0)
{
    Detail::BuildDescriptor::Check();

    Initialize();
}


inline Framework::~Framework()
{
    Release();
}


template <typename ValueType>
THERON_FORCEINLINE bool Framework::Send(const ValueType &value, const Address &from, const Address &address)
{
    // We use a thread-safe per-framework message cache to allocate messages sent from non-actor code.
    IAllocator *const messageAllocator(&mMessageAllocator);

    // Allocate a message. It'll be deleted by the worker thread that handles it.
    Detail::IMessage *const message(Detail::MessageCreator::Create(messageAllocator, value, from));
    if (message == 0)
    {
        return false;
    }

    // Call the message sending implementation using the processor context of the framework.
    // When messages are sent using Framework::Send there's no obvious worker thread.
    return SendInternal(
        &mSharedMailboxContext,
        message,
        address);
}


THERON_FORCEINLINE void Framework::SetMaxThreads(const uint32_t count)
{
    mScheduler->SetMaxThreads(count);
}


THERON_FORCEINLINE void Framework::SetMinThreads(const uint32_t count)
{
    mScheduler->SetMinThreads(count);
}


THERON_FORCEINLINE uint32_t Framework::GetMaxThreads() const
{
    return mScheduler->GetMaxThreads();
}


THERON_FORCEINLINE uint32_t Framework::GetMinThreads() const
{
    return mScheduler->GetMinThreads();
}


THERON_FORCEINLINE uint32_t Framework::GetNumThreads() const
{
    return mScheduler->GetNumThreads();
}


THERON_FORCEINLINE uint32_t Framework::GetPeakThreads() const
{
    return mScheduler->GetPeakThreads();
}


THERON_FORCEINLINE uint32_t Framework::GetNumCounters() const
{
#if THERON_ENABLE_COUNTERS
    return Detail::MAX_COUNTERS;
#else
    return 0;
#endif
}


THERON_FORCEINLINE const char *Framework::GetCounterName(const uint32_t counter) const
{
    if (counter < Detail::MAX_COUNTERS)
    {
#if THERON_ENABLE_COUNTERS
        switch (counter)
        {
            case Detail::COUNTER_MESSAGES_PROCESSED:        return "messages processed";
            case Detail::COUNTER_YIELDS:                    return "thread yields";
            case Detail::COUNTER_LOCAL_PUSHES:              return "mailboxes pushed to thread-local message queue";
            case Detail::COUNTER_SHARED_PUSHES:             return "mailboxes pushed to per-framework message queue";
            case Detail::COUNTER_MAILBOX_QUEUE_MAX:         return "maximum size of mailbox queue";
            case Detail::COUNTER_QUEUE_LATENCY_LOCAL_MIN:   return "minimum observed latency of thread-local queue";
            case Detail::COUNTER_QUEUE_LATENCY_LOCAL_MAX:   return "maximum observed latency of thread-local queue";
            case Detail::COUNTER_QUEUE_LATENCY_SHARED_MIN:  return "minimum observed latency of per-framework queue";
            case Detail::COUNTER_QUEUE_LATENCY_SHARED_MAX:  return "maximum observed latency of per-framework queue";
            default: return "unknown";
        }
#endif
    }

    return "unknown";
}


THERON_FORCEINLINE void Framework::ResetCounters()
{
#if THERON_ENABLE_COUNTERS
    mScheduler->ResetCounters();
#endif
}


THERON_FORCEINLINE uint32_t Framework::GetCounterValue(const uint32_t counter) const
{
    if (counter < Detail::MAX_COUNTERS)
    {
#if THERON_ENABLE_COUNTERS
        return mScheduler->GetCounterValue(counter);
#endif
    }

    return 0;
}


THERON_FORCEINLINE uint32_t Framework::GetPerThreadCounterValues(
    const uint32_t counter,
    uint32_t *const perThreadCounts,
    const uint32_t maxCounts) const
{
    if (counter < Detail::MAX_COUNTERS && perThreadCounts && maxCounts)
    {
#if THERON_ENABLE_COUNTERS
        return mScheduler->GetPerThreadCounterValues(counter, perThreadCounts, maxCounts);
#endif
    }

    return 0;
}


THERON_FORCEINLINE bool Framework::SendInternal(
    Detail::MailboxContext *const mailboxContext,
    Detail::IMessage *const message,
    Address address)
{
    // Index of zero implies the actor is addressed only by name and may be remote.
    if (address.mIndex.mUInt32 == 0)
    {
        const Detail::String &name(address.GetName());

        THERON_ASSERT(!name.IsNull());
        THERON_ASSERT_MSG(mEndPoint, "Sending messages addressed by name requires a Theron::EndPoint");

        // Search the local endPoint for a mailbox with the given name.
        // If there is a local match we fall through using the retrieved index, and we don't
        // bother to push the message out onto the network, since names are globally unique.
        if (!mEndPoint->Lookup(name, address.mIndex))
        {
            // If there isn't a local match we send the message out onto the network.
            return mEndPoint->RequestSend(message, name);
        }
    }

    // The address should have been resolved to a non-zero local index.
    THERON_ASSERT(address.mIndex.mUInt32);

    // Is the addressed entity in the local framework?
    if (address.mIndex.mComponents.mFramework == mIndex)
    {
        // Message is addressed to an actor in the sending framework.
        // Get a reference to the destination mailbox.
        Detail::Mailbox &mailbox(mMailboxes.GetEntry(address.mIndex.mComponents.mIndex));

        // Push the message into the mailbox and schedule the mailbox for processing
        // if it was previously empty, so won't already be scheduled.
        // The message will be destroyed by the worker thread that does the processing,
        // even if it turns out that no actor is registered with the mailbox.
        mailbox.Lock();

        const bool schedule(mailbox.Empty());
        mailbox.Push(message);

        if (schedule)
        {
            mScheduler->Schedule(mailboxContext, &mailbox);
        }

        mailbox.Unlock();

        return true;
    }

    // Message is addressed to a mailbox in the local process but not in the
    // sending Framework. In this less common case we pay the hit of an extra call.
    if (DeliverWithinLocalProcess(message, address.mIndex))
    {
        return true;
    }

    // Destroy the undelivered message.
    mFallbackHandlers.Handle(message);
    Detail::MessageCreator::Destroy(&mMessageAllocator, message);

    return false;
}


THERON_FORCEINLINE bool Framework::FrameworkReceive(
    Detail::IMessage *const message,
    const Address &address)
{
    // Call the generic message sending function.
    // We use our own local context here because we're receiving the message.
    return SendInternal(
        &mSharedMailboxContext,
        message,
        address);
}


THERON_FORCEINLINE Detail::MailboxContext *Framework::GetMailboxContext()
{
    return &mSharedMailboxContext;
}


template <typename ObjectType>
inline bool Framework::SetFallbackHandler(
    ObjectType *const handlerObject,
    void (ObjectType::*handler)(const Address from))
{
    return mFallbackHandlers.Set(handlerObject, handler);
}


template <typename ObjectType>
inline bool Framework::SetFallbackHandler(
    ObjectType *const handlerObject,
    void (ObjectType::*handler)(const void *const data, const uint32_t size, const Address from))
{
    return mFallbackHandlers.Set(handlerObject, handler);
}


} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_FRAMEWORK_H
