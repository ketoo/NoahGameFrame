// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_TRANSPORT_OUTPUTMESSAGE_H
#define THERON_DETAIL_TRANSPORT_OUTPUTMESSAGE_H


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


class OutputSocket;


/**
\brief Network output message wrapper.

Currently implemented with Crossroads.io (libxs).
*/
class OutputMessage
{
public:

    friend class OutputSocket;

    /**
    \brief Constructor.
    Constructs an output message for use with the given network context.
    An output message can be used to hold message data written to an OutputSocket.
    */
    inline explicit OutputMessage(Context *const /*context*/)
    {
    }

    /**
    \brief Initializes an OutputMessage.
    An OutputMessage must be initialized before use.
    \param size The size of the message data to be written, in bytes.
    \return True, if the message was initialized without error.
    \note A single OutputMessage can be reused multiple times by repeatedly calling Initialize and Release.
    */
    inline bool Initialize(const uint32_t size)
    {
        (void) size;

#if THERON_XS

        // Initialize the XS message.
        if (xs_msg_init_size(&mMessage, size) == 0)
        {
            return true;
        }

#endif // THERON_XS

        return false;
    }

    /**
    \brief Releases data held by an OutputMessage.
    An OutputMessage must be released after use and prior to destruction.
    \return True, if the message was released without error.
    \note A single OutputMessage can be reused multiple times by repeatedly calling Initialize and Release.
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
    \brief Returns a pointer to the message data held in an OutputMessage.
    Call this method before writing a message to an OutputSocket using OutputSocket::Send.
    After acquiring a pointer to the data block the caller can write the message data into it.
    \return A void pointer to the data block owned by the message.
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


#endif // THERON_DETAIL_TRANSPORT_OUTPUTMESSAGE_H
