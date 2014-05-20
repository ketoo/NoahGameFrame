// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_ADDRESS_H
#define THERON_ADDRESS_H


/**
\file Address.h
Address object by which messages are addressed.
*/


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Network/Index.h>
#include <Theron/Detail/Strings/String.h>


namespace Theron
{


class EndPoint;
class Framework;
class Receiver;


/**
\brief The unique address of an entity that can send or receive messages.

Addresses are the unique 'names' of entities that can receive messages in Theron.
Knowing an entity's address is enough to be able to send it a message, and no other
special access or permissions are required.

The types of entities that have addresses, and therefore can receive messages, in
Theron are \ref Actor "actors" and \ref Receiver "receivers". Both of these types
of entities are assigned automatically-generated unique addresses on construction.

Addresses can be copied and assigned, allowing the addresses of actors and receivers
to be \ref Actor::Send "sent in messages" to other actors.

\note Addresses can't currently be sent in messages to remote actors over the network,
since they can't be copied trivially by means of memcpy.

Default-constructed addresses are \em null and are equal to the unique null address
returned by the static \ref Address::Null method, which is guaranteed not to be
equal to the address of any addressable entity.

Because knowing the address of an actor means being able to send it a message,
addresses constitute the security and encapsulation mechanism within \ref Actor "actor"
subsystems. If an actor creates a collection of 'child' actors that it owns and manages,
then no code outside that actor can access the child actors or send them messages,
unless the actor publicly exposes either the actors or their addresses.
*/
class Address
{
public:

    friend class EndPoint;
    friend class Framework;
    friend class Receiver;

    /**
    \brief Static method that returns the unique 'null' address.

    The null address is guaranteed not to be equal to the address of any actual entity.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    assert(actor.GetAddress() != Address::Null(), "Created actor has null address!");
    \endcode

    \return The unique null address.
    */
    THERON_FORCEINLINE static const Address &Null()
    {
        return smNullAddress;
    }

    /**
    \brief Default constructor.

    Constructs a null address, equal to the address returned by Address::Null().
    */
    THERON_FORCEINLINE Address() : mName(), mIndex()
    {
    }

    /**
    \brief Explicit constructor.

    Constructs an address with a specific string name. Use this to send messages to remote actors,
    in other processes or on other hosts, that are known only by their unique names.
    */
    THERON_FORCEINLINE explicit Address(const char *const name) : mName(name), mIndex()
    {
    }

    /**
    \brief Copy constructor.

    Addresses can be assigned. The new address is equal to the original and identifies the same entity.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    // Copy-construct the actor's address.
    Theron::Address address(actor.GetAddress());
    \endcode

    \param other The existing address to be copied.
    */
    THERON_FORCEINLINE Address(const Address &other) :
      mName(other.mName),
      mIndex(other.mIndex)
    {
    }

    /**
    \brief Assignment operator.

    Addresses can be assigned. The new address is equal to the original and identifies the same entity.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);
    
    // Assign the actor's address into a default-constructed address.
    Theron::Address address;
    address = actor.GetAddress();
    \endcode

    \param other The existing address whose value is to be copied.
    */
    THERON_FORCEINLINE Address &operator=(const Address &other)
    {
        mName = other.mName;
        mIndex = other.mIndex;
        return *this;
    }

    /**
    \brief Gets an integer index identifying the framework containing the addressed entity.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    printf("Actor is in framework %lu\n", actor.GetAddress().GetFramework());
    \endcode

    \note A value of zero indicates that the entity is a Receiver, which is global to the
    current process and not associated with any specific framework.
    */
    THERON_FORCEINLINE uint32_t GetFramework() const
    {
        return mIndex.mComponents.mFramework;
    }

    /**
    \brief Gets the value of the address as a string.

    Actor and receiver mailboxes have string names, which by default are randomly
    generated. The name of each mailbox should be globally unique across all connected
    processes, and allows the mailbox to be sent messages by actors which don't
    know its physical location.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    printf("Actor has address %s\n", actor.GetAddress().AsString());
    \endcode
    */
    THERON_FORCEINLINE const char *AsString() const
    {
        return mName.GetValue();
    }

    /**
    \brief Gets the value of the address as an unsigned 32-bit integer.

    \note The returned integer value of an address represents its index within the
    framework within which it was created, and isn't unique across multiple frameworks
    within the same process, across multiple processes within the same host, or across
    multiple hosts. In order to uniquely characterize actors across all frameworks within
    a process, either use \ref AsUInt64 or combine the value returned by AsInteger with
    the value returned by \ref GetFramework, which uniquely identifies the framework
    hosting the actor within the current process.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    printf("Actor has address %lu\n", actor.GetAddress().AsInteger());
    \endcode

    \see AsUInt64
    */
    THERON_FORCEINLINE uint32_t AsInteger() const
    {
        return mIndex.mComponents.mIndex;
    }

    /**
    \brief Gets the unique value of the address as an unsigned 64-bit integer.

    \note The 64-bit value of an address is not guaranteed to be unique across multiple
    hosts or multiple processes within the same host. Use the \ref AsString "string value"
    of an address if you require globally unique identifiers.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actor(framework);

    printf("Actor has address %llu\n", actor.GetAddress().AsUInt64());
    \endcode

    \see AsInteger
    */
    THERON_FORCEINLINE uint64_t AsUInt64() const
    {
        return static_cast<uint64_t>(mIndex.mUInt32);
    }

    /**
    \brief Equality operator.

    Returns true if two addresses are the same, otherwise false.

    \param other The address to be compared.
    \return True if the given address is the same as this one.
    */
    THERON_FORCEINLINE bool operator==(const Address &other) const
    {
        return (mName == other.mName);
    }

    /**
    \brief Inequality operator.

    Returns true if two addresses are different, otherwise false.

    \param other The address to be compared.
    \return True if the given address is different from this one.
    */
    THERON_FORCEINLINE bool operator!=(const Address &other) const
    {
        return !operator==(other);
    }

    /**
    \brief Less-than operator.

    This allows addresses to be sorted, for example in containers.

    \code
    class Actor : public Theron::Actor
    {
        explicit Actor(Theron::Framework &framework) : Theron::Actor(framework)
        {
        }
    };

    Theron::Framework framework;
    Actor actorOne(framework);
    Actor actorTwo(framework);

    std::set<Theron::Address> addresses;
    addresses.insert(actorOne.GetAddress());
    addresses.insert(actorTwo.GetAddress());
    \endcode
    */
    THERON_FORCEINLINE bool operator<(const Address &other) const
    {
        return (mName < other.mName);
    }

private:

    /**
    Internal explicit constructor, used by friend classes.
    */
    THERON_FORCEINLINE explicit Address(const Detail::String &name, const Detail::Index &index) :
       mName(name),
       mIndex(index)
    {
    }

    THERON_FORCEINLINE const Detail::String &GetName() const
    {
        return mName;
    }

    static Address smNullAddress;       ///< A single static instance of the null address.

    Detail::String mName;               ///< The string name of the addressed entity.
    Detail::Index mIndex;               ///< Cached local framework and index.
};


} // namespace Theron


#endif // THERON_ADDRESS_H
