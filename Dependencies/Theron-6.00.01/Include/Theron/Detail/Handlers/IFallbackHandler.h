// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_IFALLBACKHANDLER_H
#define THERON_DETAIL_HANDLERS_IFALLBACKHANDLER_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{
namespace Detail
{


/**
Baseclass that allows fallback handlers to be stored in lists.
*/
class IFallbackHandler : public List<IFallbackHandler>::Node
{
public:

    /**
    Default constructor.
    */
    THERON_FORCEINLINE IFallbackHandler()
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~IFallbackHandler()
    {
    }

    /**
    Handles the given message.
    */
    virtual void Handle(const IMessage *const message) const = 0;

private:

    IFallbackHandler(const IFallbackHandler &other);
    IFallbackHandler &operator=(const IFallbackHandler &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_IFALLBACKHANDLER_H

