// -------------------------------------------------------------------------
//    @FileName      :    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_H_
#define _NFI_ACTOR_H_

#include <map>
#include <string>

#ifdef NF_USE_ACTOR
#include "Theron/Receiver.h"
#include "Theron/Actor.h"
#include "Theron/Address.h"
#include "Theron/Framework.h"
#include "Theron/Detail/Handlers/ReceiverHandler.h"

#include "NFILogicModule.h"
#include "NFIActorManager.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFIComponent.h"

class NFIActorMessage
{
public:
    NFIActorMessage()
    {
        eType = EACTOR_UNKNOW;
        nSubMsgID = 0;
    }

	enum EACTOR_MESSAGE_ID
	{
        EACTOR_UNKNOW,
		EACTOR_INIT,
		EACTOR_AFTER_INIT,
        EACTOR_CHECKCONFIG,
		EACTOR_EXCUTE,
		EACTOR_BEFORE_SHUT,
		EACTOR_SHUT,
		EACTOR_NET_MSG,
        EACTOR_TRANS_MSG,
		EACTOR_LOG_MSG,
		EACTOR_EVENT_MSG,
		EACTOR_RETURN_EVENT_MSG,
	};

    EACTOR_MESSAGE_ID eType;
    int nSubMsgID;
	std::string data;
	////////////////////event/////////////////////////////////////////////////
	NFIDENTID self;
	NF_SHARE_PTR<NFAsyncEventFunc> xActorEventFunc;//包含同步异步等回调接口
	//////////////////////////////////////////////////////////////////////////
protected:
private:
};

class NFIActor : public Theron::Actor, public NFILogicModule
{
public:
	NFIActor(Theron::Framework &framework, NFIActorManager* pManager) : Theron::Actor(framework)
    {
        m_pActorManager = pManager;

        RegisterHandler(this, &NFIActor::Handler);
    }

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from){};
	NFIActorManager* GetActorManager(){return m_pActorManager;}

	virtual void RegisterActorComponent(const std::string& strModuleName, NFIComponent* pComponent) = 0;

private:
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from)
    {
		//收到消息要处理逻辑
		if (message.eType == NFIActorMessage::EACTOR_EVENT_MSG)
		{
			std::string strData = message.data;
			message.xActorEventFunc->xBeginFuncptr->operator()(message.self, message.nSubMsgID, strData);
			
			////////////////////////////////////////////////////////

			NFIActorMessage xReturnMessage;

			xReturnMessage.eType = NFIActorMessage::EACTOR_RETURN_EVENT_MSG;
			xReturnMessage.nSubMsgID = message.nSubMsgID;
			xReturnMessage.data = strData;
			////////////////////event/////////////////////////////////////////////////
			xReturnMessage.self = message.self;
			xReturnMessage.xActorEventFunc = message.xActorEventFunc;

			Send(xReturnMessage, from);
		}
		else
		{
			HandlerEx(message, from);
		}
	}

protected:
    NFIActorManager* m_pActorManager;

};

#endif

#endif