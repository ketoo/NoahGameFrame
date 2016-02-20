// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_IRECEIVERHANDLER_H
#define THERON_DETAIL_HANDLERS_IRECEIVERHANDLER_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{
namespace Detail
{


/**
Baseclass that allows message handlers of various types to be stored in lists.
*/
class IReceiverHandler : public List<IReceiverHandler>::Node
{
public:

    /**
    Default constructor.
    */
    THERON_FORCEINLINE IReceiverHandler()
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~IReceiverHandler()
    {
    }

    /**
    Returns the unique name of the message type handled by this handler.
    */
    virtual const char *GetMessageTypeName() const = 0;

    /**
    Handles the given message, if it's of the type accepted by the handler.
    \return True, if the handler handled the message.
    */
    virtual bool Handle(const IMessage *const message) const = 0;

private:

    IReceiverHandler(const IReceiverHandler &other);
    IReceiverHandler &operator=(const IReceiverHandler &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_IRECEIVERHANDLER_H
