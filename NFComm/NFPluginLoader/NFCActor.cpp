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
    //添加到主线程事件
    m_pActorManager->GetPluginManager()->HandlerEx(message, from);
}

void NFCActor::AddComponent(NF_SHARE_PTR<NFIComponent> pComponent)
{
    if (nullptr != pComponent)
    {
        mxComponentList.Add(pComponent);
    }
}

void NFCActor::HandlerSelf(const NFIActorMessage& message, const Theron::Address from)
{
    std::string strData = message.data;

    NF_SHARE_PTR<NFIComponent> pComponent;
    bool bRet = mxComponentList.First(pComponent);
    while (bRet)
    {
        if (!pComponent->HasInit())
        {
            pComponent->SetHasInit(true);

            pComponent->Init();
            pComponent->AfterInit();
        }

        pComponent->OnASyncEvent(message.self, message.nSubMsgID, strData);

        bRet = mxComponentList.Next(pComponent);
    }
    //m_pComponent->OnASyncEvent(message.self, message.nSubMsgID, strData);

    //message.xActorEventFunc->xBeginFuncptr->operator()(message.self, message.nSubMsgID, strData);

    ////////////////////////////////////////////////////////

    NFIActorMessage xReturnMessage;

    xReturnMessage.eType = NFIActorMessage::EACTOR_RETURN_EVENT_MSG;
    xReturnMessage.nSubMsgID = message.nSubMsgID;
    xReturnMessage.data = strData;
    ////////////////////event/////////////////////////////////////////////////
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
