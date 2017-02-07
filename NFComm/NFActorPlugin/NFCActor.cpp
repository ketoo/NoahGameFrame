// -------------------------------------------------------------------------
//    @FileName			:    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

void NFCActor::HandlerEx(const NFIActorMessage& message, const Theron::Address from)
{
    m_pActorModule->HandlerEx(message, from.AsInteger());
}

void NFCActor::AddComponent(NF_SHARE_PTR<NFIComponent> pComponent)
{
	mxComponent = pComponent;
}

void NFCActor::HandlerSelf(const NFIActorMessage& message, const Theron::Address from)
{
    std::string strData = message.data;

    if (mxComponent)
    {
        if (!mxComponent->HasInit())
        {
			mxComponent->SetHasInit(true);

			mxComponent->Init();
			mxComponent->AfterInit();
        }

		mxComponent->OnASyncEvent(message.self, message.nMsgID, strData);

    }
 
    ////////////////////////////////////////////////////////

    NFIActorMessage xReturnMessage;

	xReturnMessage.bComponentMsg = false;
	xReturnMessage.nMsgID = message.nMsgID;
    xReturnMessage.data = strData;
    xReturnMessage.self = message.self;
    xReturnMessage.nFormActor = this->GetAddress().AsInteger();
    xReturnMessage.xEndFuncptr = mxFunctorEndPtr;

    Send(xReturnMessage, from);
}

bool NFCActor::AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end)
{
    mxFunctorEndPtr = functorPtr_end;
    return true;
}

bool NFCActor::SendMsg(const Theron::Address address, const NFIActorMessage& message)
{
    return Send(message, address);
}
