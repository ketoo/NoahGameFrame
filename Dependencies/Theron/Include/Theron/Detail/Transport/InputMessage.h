// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_TRANSPORT_INPUTMESSAGE_H
#define THERON_DETAIL_TRANSPORT_INPUTMESSAGE_H


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


class InputSocket;


/**
\brief Network input message wrapper.

Currently implemented with Crossroads.io (libxs).
*/
class InputMessage
{
public:

    friend class InputSocket;

    /**
    \brief Constructor.
    Constructs an input message for use with the given network context.
    An input message can be used to hold message data read from an InputSocket.
    */
    inline explicit InputMessage(Context *const /*context*/)
    {
    }

    /**
    \brief Initializes an InputMessage.
    An InputMessage must be initialized before use.
    \return True, if the message was initialized without error.
    \note A single InputMessage can be reused multiple times by repeatedly calling Initialize and Release.
    */
    inline bool Initialize()
    {
#if THERON_XS

        // Initialize the XS message.
        if (xs_msg_init(&mMessage) == 0)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

    /**
    \brief Releases data held by an InputMessage.
    An InputMessage must be released after use and prior to destruction.
    \return True, if the message was released without error.
    \note A single InputMessage can be reused multiple times by repeatedly calling Initialize and Release.
    */
    inline bool Release()
    {
#if THERON_XS

        // Close the XS message.
        if (xs_msg_close(&mMessage) == 0)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

    /**
    \brief Returns the size of the message data held in an InputMessage.
    Call this method after reading a message from an InputSocket using InputSocket::NonBlockingReceive.
    \return The size of the read message data in bytes.
    */
    inline uint32_t Size()
    {
        uint32_t size(0);

#if THERON_XS

        size = static_cast<uint32_t>(xs_msg_size(&mMessage));

#endif // THERON_XS

        return size;
    }

    /**
    \brief Returns a pointer to the message data held in an InputMessage.
    Call this method after reading a message from an InputSocket using InputSocket::NonBlockingReceive.
    */
    inline void *Data()
    {
        void *data(0);

#if THERON_XS

        data = xs_msg_data(&mMessage);

#endif // THERON_XS

        return data;
    }

private:

#if THERON_XS
    xs_msg_t mMessage;      ///< XS message.
#endif // THERON_XS

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_TRANSPORT_INPUTMESSAGE_H
