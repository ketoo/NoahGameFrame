// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_HANDLERS_IMESSAGEHANDLER_H
#define THERON_DETAIL_HANDLERS_IMESSAGEHANDLER_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Messages/IMessage.h>


namespace Theron
{


class Actor;


namespace Detail
{


/**
Baseclass that allows message handlers of various types to be stored in lists.
*/
class IMessageHandler : public List<IMessageHandler>::Node
{
public:

    /**
    Default constructor.
    */
    THERON_FORCEINLINE IMessageHandler() : mMarked(false), mPredictedSendCount(0)
    {
    }

    /**
    Virtual destructor.
    */
    inline virtual ~IMessageHandler()
    {
    }

    /**
    Marks the handler (eg. for deletion).
    */
    inline void Mark();

    /**
    Returns true if the handler is marked (eg. for deletion).
    */
    inline bool IsMarked() const;

    /**
    Reports the number of messages sent by an invocation of the handler.
    */
    inline void ReportSendCount(const uint32_t count);

    /**
    Gets a prediction of the number of messages that would be sent by the handler when invoked.
    */
    inline uint32_t GetPredictedSendCount() const;

    /**
    Returns the unique name of the message type handled by this handler.
    */
    virtual const char *GetMessageTypeName() const = 0;

    /**
    Handles the given message, if it's of the type accepted by the handler.
    \return True, if the handler handled the message.
    */
    virtual bool Handle(Actor *const actor, const IMessage *const message) = 0;

private:

    IMessageHandler(const IMessageHandler &other);
    IMessageHandler &operator=(const IMessageHandler &other);

    bool mMarked;                   ///< Flag used to mark the handler for deletion.
    uint32_t mPredictedSendCount;   ///< Number of messages that are predicted to be sent by the handler.
};


THERON_FORCEINLINE void IMessageHandler::Mark()
{
    mMarked = true;
}


THERON_FORCEINLINE bool IMessageHandler::IsMarked() const
{
    return mMarked;
}


THERON_FORCEINLINE void IMessageHandler::ReportSendCount(const uint32_t count)
{
    // For now we assume that the message send count will be the same as last time.
    if (mPredictedSendCount != count)
    {
        mPredictedSendCount = count;
    }
}


THERON_FORCEINLINE uint32_t IMessageHandler::GetPredictedSendCount() const
{
    return mPredictedSendCount;
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_HANDLERS_IMESSAGEHANDLER_H
