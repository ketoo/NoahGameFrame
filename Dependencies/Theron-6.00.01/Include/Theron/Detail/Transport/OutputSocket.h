// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_TRANSPORT_OUTPUTSOCKET_H
#define THERON_DETAIL_TRANSPORT_OUTPUTSOCKET_H


#include <Theron/Defines.h>

// NOTE: Must include xs.h before standard headers to avoid warnings in MS headers!
#if THERON_XS
#include <xs/xs.h>
#endif // THERON_XS

#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>

#include <Theron/Detail/Transport/Context.h>
#include <Theron/Detail/Transport/OutputMessage.h>


namespace Theron
{
namespace Detail
{


/**
\brief Network output socket wrapper.

Currently implemented with Crossroads.io (libxs).
*/
class OutputSocket
{
public:

    /**
    \brief Constructs a new output socket within the given network context.
    */
    inline explicit OutputSocket(Context *const context)
    {
        (void) context;

#if THERON_XS

        mSocket = xs_socket(context->mContext, XS_PUB);
        if (mSocket == 0)
        {
            THERON_FAIL_MSG("Failed to create XS publish socket");
        }

#endif // THERON_XS
    }

    /**
    \brief Destructs an OutputSocket.
    */
    inline ~OutputSocket()
    {
#if THERON_XS

        THERON_ASSERT(mSocket);

        // Close the XS socket.
        if (xs_close(mSocket) != 0)
        {
            THERON_FAIL_MSG("Failed to close XS publish socket");
        }

#endif // THERON_XS
    }

    /**
    \brief Binds an OutputSocket socket to a local endpoint.
    The output socket is implicitly connected to any remote InputSocket instances bound to the local endpoint.
    \param location A string identifying the local endpoint, currently in XS format.
    \return True, if the socket was bound without error.
    */
    inline bool Bind(const char *const location)
    {
        (void) location;

#if THERON_XS

        THERON_ASSERT(mSocket);

        if (xs_bind(mSocket, location) != -1)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

    /**
    \brief Sends a message contained in the given InputMessage.
    This method sends the given message data in the given OutputMessage on the OutputSocket.
    \param outputMessage Pointer to an OutputMessage containing the message data to be sent.
    \return True, if the message was sent without error.
    \note Just because the message was sent on the OutputSocket doesn't imply that it was read
    and processed by a InputSocket connected to the endpoint to which the OutputSocket is bound.
    */
    inline bool Send(OutputMessage *const outputMessage)
    {
        (void) outputMessage;

#if THERON_XS

        // Send the network message.
        if (xs_sendmsg(mSocket, &outputMessage->mMessage, 0) >= 0)
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


#endif // THERON_DETAIL_TRANSPORT_OUTPUTSOCKET_H
