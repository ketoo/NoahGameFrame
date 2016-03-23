// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_IDEFAULTHANDLER_H
#define THERON_DETAIL_HANDLERS_IDEFAULTHANDLER_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{


class Actor;


namespace Detail
{


/**
Baseclass that allows default handlers to be stored in lists.
*/
class IDefaultHandler
{
public:

    /**
    Default constructor.
    */
    inline IDefaultHandler()
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~IDefaultHandler()
    {
    }

    /**
    Handles the given message.
    */
    virtual void Handle(Actor *const actor, const IMessage *const message) const = 0;

private:

    IDefaultHandler(const IDefaultHandler &other);
    IDefaultHandler &operator=(const IDefaultHandler &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_IDEFAULTHANDLER_H
