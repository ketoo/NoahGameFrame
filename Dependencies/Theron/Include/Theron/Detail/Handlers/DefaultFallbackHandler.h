// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_DEFAULTFALLBACKHANDLER_H
#define THERON_DETAIL_HANDLERS_DEFAULTFALLBACKHANDLER_H


#if THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS
#include <stdio.h>
#endif // THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS

#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


#if THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS
#define THERON_FALLBACK_HANDLER_ARG(x) x
#else
#define THERON_FALLBACK_HANDLER_ARG(x)
#endif //THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS


namespace Theron
{
namespace Detail
{


class DefaultFallbackHandler
{
public:

    inline void Handle(const void *const data, const uint32_t size, const Address from);
};


inline void DefaultFallbackHandler::Handle(
    const void *const THERON_FALLBACK_HANDLER_ARG(data),
    const uint32_t THERON_FALLBACK_HANDLER_ARG(size),
    const Address THERON_FALLBACK_HANDLER_ARG(from))
{
#if THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS

    fprintf(stderr, "Unhandled message of %d bytes sent from address %d:\n", size, from.AsInteger());

    // Dump the message data as hex words.
    if (data)
    {
        const char *const format("[%d] 0x%08x\n");

        const unsigned int *const begin(reinterpret_cast<const unsigned int *>(data));
        const unsigned int *const end(begin + size / sizeof(unsigned int));

        for (const unsigned int *word(begin); word != end; ++word)
        {
            fprintf(stderr, format, static_cast<int>(word - begin), static_cast<int>(*word));
        }
    }

    THERON_FAIL();

#endif // THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS
}


} // namespace Detail
} // namespace Theron


#undef THERON_FALLBACK_HANDLER_ARG


#endif // THERON_DETAIL_HANDLERS_DEFAULTFALLBACKHANDLER_H
