// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_TRANSPORT_INPUTSOCKET_H
#define THERON_DETAIL_TRANSPORT_INPUTSOCKET_H


#include <Theron/Defines.h>

// NOTE: Must include xs.h before standard headers to avoid warnings in MS headers!
#if THERON_XS
#include <xs/xs.h>
#endif // THERON_XS

#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>

#include <Theron/Detail/Transport/Context.h>


namespace Theron
{
namespace Detail
{


/**
\brief Network input socket wrapper.

Currently implemented with Crossroads.io (libxs).
*/
class InputSocket
{
public:

    /**
    \brief Constructs a new input socket within the given network context.
    */
    inline explicit InputSocket(Context *const context)
    {
        (void) context;

#if THERON_XS

        mSocket = xs_socket(context->mContext, XS_SUB);
        if (mSocket == 0)
        {
            THERON_FAIL_MSG("Failed to create XS subscribe socket");
        }

        // TODO: We could be smart and only subscribe to messages we care about.
        // Set a null subscribe filter on the subscribe socket so it receives all published messages.
        // This must be done before the socket is connected.
        if (xs_setsockopt(mSocket, XS_SUBSCRIBE, "", 0) != 0)
        {
            THERON_FAIL_MSG("Failed to set XS socket subscribe option");
        }

#endif // THERON_XS
    }

    /**
    \brief Destructs an InputSocket.
    */
    inline ~InputSocket()
    {
#if THERON_XS

        THERON_ASSERT(mSocket);

        // Close the XS socket.
        if (xs_close(mSocket) != 0)
        {
            THERON_FAIL_MSG("Failed to close XS subscribe socket");
        }

#endif // THERON_XS
    }

    /**
    \brief Connects an InputSocket socket to a remote endpoint.
    The input socket is implicitly connected to any OutputSocket instances bound locally to the remote endpoint.
    \param location A string identifying the remote endpoint, currently in XS format.
    \return True if the connection request was successfully created.
    \note The return value of this function doesn't imply that the connection will actually be made, merely that it was requested.
    */
    inline bool Connect(const char *const location)
    {
        (void) location;

#if THERON_XS

        THERON_ASSERT(mSocket);

        // Connect the subscribe socket to the remote host.
        // The same socket can be connected to multiple remote endpoints.
        if (xs_connect(mSocket, location) != -1)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

    /**
    \brief Tries to receive a message into the given InputMessage, without blocking.
    This method reads the first available message from the socket into the given InputMessage.
    This method is a non-blocking call and doesn't block or what for a message to arrive.
    \param inputMessage Pointer to an InputMessage to be filled with the read message data.
    \return True, if a message was available and successfully read.
    */
    inline bool NonBlockingReceive(InputMessage *const inputMessage)
    {
        (void) inputMessage;

#if THERON_XS

        // Send the network message.
        if (xs_recvmsg(mSocket, &inputMessage->mMessage, XS_DONTWAIT) != -1)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

private:

#if THERON_XS
    void *mSocket;      ///< Void pointer to an XS socket.
#endif // THERON_XS

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_TRANSPORT_INPUTSOCKET_H
