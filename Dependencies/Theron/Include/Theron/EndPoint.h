// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_ENDPOINT_H
#define THERON_ENDPOINT_H


/**
\file EndPoint.h
An endpoint in a connected network of processes, potentially on remote hosts.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <Theron/Address.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/Register.h>

#include <Theron/Detail/Containers/Queue.h>
#include <Theron/Detail/Messages/IMessage.h>
#include <Theron/Detail/Network/MessageFactory.h>
#include <Theron/Detail/Network/NameMap.h>
#include <Theron/Detail/Strings/String.h>
#include <Theron/Detail/Strings/StringPool.h>
#include <Theron/Detail/Threading/Mutex.h>
#include <Theron/Detail/Threading/Thread.h>
#include <Theron/Detail/Transport/Context.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4127)  // conditional expression is constant
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{


class Actor;
class Framework;
class Receiver;


/**
\brief Network communication object.

Enables communication with actors in other processes and on remote hosts.
A single instance of this class must be constructed in each application that
is intended to be used in a collection of distributed applications in distributed
computing.

If an EndPoint is created then it must outlive any frameworks, actors and receivers
that are tied to it.
*/
class EndPoint
{
public:

    friend class Actor;
    friend class Framework;
    friend class Receiver;

    /**
    \brief Parameters structure that can be passed to the EndPoint constructor.
    Currently this is empty and is here just to allow future expansion.
    */
    struct Parameters
    {
    };

    /**
    \brief Explicit constructor.

    Constructs a EndPoint with the given parameters.

    \param name String name for the endpoint, required to be unique.
    \param location String defining the network location to which the endpoint is bound.
    \param params Optional parameters object containing initialization parameters.

    \note The name defined on construction should be globally unique across all connected hosts.
    The name and location parameters are copied, so can be destroyed after the call.

    \code
    int main(int argc, char *argv[])
    {
        // Create a local endpoint using a local IP address provided as a command-line parameter.
        char buffer[256];
        sprintf(buffer, "tcp://%s:5556", argv[1]);
        Theron::EndPoint endPoint("client", buffer);

        // Connect to the remote endpoint at an IP address provided as a second command-line parameter.
        sprintf(buffer, "tcp://%s:5555", argv[2]);
        if (!endPoint.Connect(buffer))
        {
            printf("ERROR: Connection failed - check networking is enabled.\n");
            return 1;
        }

        // Construct a framework tied to the local endpoint.
        Theron::Framework framework(endPoint);
    }
    \endcode
    */
    EndPoint(const char *const name, const char *const location, const Parameters params = Parameters());

    /**
    Destructor.
    */
    ~EndPoint();

    /**
    \brief Gets the name of the EndPoint instance.
    The name is defined on construction and should be globally unique across all connected hosts.
    */
    inline const char *GetName() const;

    /**
    \brief Connects this EndPoint object to another (remote) EndPoint object.

    The remote EndPoint is typically on another host, connected to the local one by a
    network connection. It is also possible to use this mechanism to connect different
    processes on the same host.

    The format of the address parameter is "tcp://192.168.10.104:5555" where "192.168.10.104" is the
    IP address of the remote host and "5555" is the number of the network port on the remote host
    to which the remote EndPoint is assigned.

    \param address Unique network address (IP address and port) of the remote EndPoint instance.
    \return True, if the connection succeeded.

    \code
    int main(int argc, char *argv[])
    {
        // Create a local endpoint using a local IP address provided as a command-line parameter.
        char buffer[256];
        sprintf(buffer, "tcp://%s:5556", argv[1]);
        Theron::EndPoint endPoint("client", buffer);

        // Connect to the remote endpoint at an IP address provided as a second command-line parameter.
        sprintf(buffer, "tcp://%s:5555", argv[2]);
        if (!endPoint.Connect(buffer))
        {
            printf("ERROR: Connection failed - check networking is enabled.\n");
            return 1;
        }

        // Construct a framework tied to the local endpoint.
        Theron::Framework framework(endPoint);
    }
    \endcode
    */
    bool Connect(const char *const address);

private:

    // Request structure used to represent a request to connect to a remote endpoint.
    class ConnectRequest : public Detail::Queue<ConnectRequest>::Node
    {
    public:

        inline explicit ConnectRequest(const char *const location) : mLocation(location)
        {
        }

        Detail::String mLocation;        ///< The endpoint address in host::port format.

    private:

        ConnectRequest(const ConnectRequest &other);
        ConnectRequest &operator=(const ConnectRequest &other);
    };

    // Request structure used to represent a request to send a message to a (remote or local) name.
    class SendRequest : public Detail::Queue<SendRequest>::Node
    {
    public:

        inline explicit SendRequest(Detail::IMessage *const message, const Detail::String &name) :
          mMessage(message),
          mName(name)
        {
        }

        Detail::IMessage *const mMessage;       ///< The allocated message.
        Detail::String mName;                   ///< The destination name.

    private:

        SendRequest(const SendRequest &other);
        SendRequest &operator=(const SendRequest &other);
    };

    EndPoint(const EndPoint &other);
    EndPoint &operator=(const EndPoint &other);

    /**
    \brief Acquires a pointer to the single network context within the local process.
    All EndPoint objects created within a process share the same network context.
    This allows them to communicate using local 'inproc' network communication.
    \return Pointer to the single Context instance, which is created if need be.
    */
    static Detail::Context *InitializeContext();

    /**
    Releases a reference to the single network context within the local process.
    */
    static void ReleaseContext();

     /**
    \brief Associates a mailbox index with a given string name.
    \note Mailbox names must be unique.
    */
    inline bool Register(const Detail::String &name, const Detail::Index &index);

    /**
    \brief Removes the association of a mailbox index with the given string name.
    */
    inline bool Deregister(const Detail::String &name);

    /**
    \brief Looks up the mailbox index registered with the given name, if any.
    */
    inline bool Lookup(const Detail::String &name, Detail::Index &index) const;

    /**
    Registers a message type for serialization on the network, so it can be sent in network messages.
    The message type must have been registered by means of \ref THERON_REGISTER_MESSAGE.
    */
    template <class ValueType>
    inline bool RegisterMessageType();

    /**
    Sends the given allocated message to the mailboxes with the given published name.
    */
    bool RequestSend(Detail::IMessage *const message, const Detail::String &name);

    static void NetworkThreadEntryPoint(void *const context);
    void NetworkThreadProc();

    static Detail::Mutex smContextMutex;                ///< Ensures thread-safe access to the per-process network context.
    static uint32_t smContextRefCount;                  ///< Context object reference count.
    static Detail::Context *smContext;                  ///< Pointer the single per-process network context object.

    Detail::StringPool::Ref mStringPoolRef;             ///< Ensures that the StringPool is created.
    Detail::String mName;                               ///< Name of the this endpoint, unique across all hosts.
    Detail::String mLocation;                           ///< Unique network location (eg. IP address and port) of the endpoint.
    Detail::Context *mContext;                          ///< Pointer to per-process network context object.
    Detail::NameMap mNameMap;                           ///< Map of published names to addresses.
    Detail::MessageFactory mMessageFactory;             ///< Builds messages from serialized data blocks.
    Detail::Thread mNetworkThread;                      ///< Creates and listens to network sockets.
    bool mRunning;                                      ///< Flag used to terminate the network thread.
    bool mStarted;                                      ///< Flag used to signal that the network thread has started.
    Detail::Mutex mNetworkMutex;                        ///< Thread-safe access to network state.
    Detail::Queue<ConnectRequest> mConnectQueue;        ///< Queue of connection requests.
    Detail::Queue<SendRequest> mSendQueue;              ///< Queue of message send requests.
};


THERON_FORCEINLINE const char *EndPoint::GetName() const
{
    return mName.GetValue();
}


template <class ValueType>
inline bool EndPoint::RegisterMessageType()
{
    // Constant expression - may need to disable a warning ...
    if (Detail::MessageTraits<ValueType>::HAS_TYPE_NAME)
    {
        const Detail::String name(Detail::MessageTraits<ValueType>::TYPE_NAME);
        mMessageFactory.Register<ValueType>(name);

        return true;
    }

    return false;
}


THERON_FORCEINLINE bool EndPoint::Register(const Detail::String &name, const Detail::Index &index)
{
    // Remove any existing entry for the name.
    if (!mNameMap.Remove(name))
    {
        return false;
    }

    // Insert the new entry.
    if (!mNameMap.Insert(name, index))
    {
        return false;
    }

    return true;
}


THERON_FORCEINLINE bool EndPoint::Deregister(const Detail::String &name)
{
    // Remove any existing entry for the name.
    if (!mNameMap.Remove(name))
    {
        return false;
    }

    return true;
}


THERON_FORCEINLINE bool EndPoint::Lookup(const Detail::String &name, Detail::Index &index) const
{
    return mNameMap.Get(name, index);
}


} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_ENDPOINT_H
