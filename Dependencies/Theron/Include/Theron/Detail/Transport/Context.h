// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_TRANSPORT_CONTEXT_H
#define THERON_DETAIL_TRANSPORT_CONTEXT_H


#include <Theron/Defines.h>

// NOTE: Must include xs.h before standard headers to avoid warnings in MS headers!
#if THERON_XS
#include <xs/xs.h>
#endif // THERON_XS

#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>


namespace Theron
{
namespace Detail
{


class InputSocket;
class OutputSocket;


/**
\brief Network context wrapper.

Currently implemented with Crossroads.io (libxs).
*/
class Context
{
public:

    friend class InputSocket;
    friend class OutputSocket;

    /**
    \brief Default constructor.
    Constructs a network context. At most one network context is created per application.
    */
    inline Context()
    {
#if THERON_XS

        // Create XS context.
        mContext = xs_init();
        if (mContext == 0)
        {
            THERON_FAIL_MSG("Failed to initialize XS context");
        }

#endif // THERON_XS
    }

    /**
    \brief Destructor.
    Destructs a network context.
    */
    inline ~Context()
    {
#if THERON_XS

        // Destroy the XS context.
        if (xs_term(mContext) != 0)
        {
            THERON_FAIL_MSG("Failed to terminate XS context");
        }

#endif // THERON_XS
    }

    /**
    \brief Returns true if network support is enabled.
    This is used to catch the case where network support is not enabled.
    */
    inline bool Enabled() const
    {
#if THERON_XS

        return (mContext != 0);

#else

        return false;

#endif // THERON_XS
    }

private:

#if THERON_XS
    void *mContext;     ///< Void pointer to an XS context.
#endif // THERON_XS

};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_TRANSPORT_CONTEXT_H
