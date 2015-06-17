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
#endif

#include "NFILogicModule.h"
#include "NFIActorManager.h"
#include "NFComm/NFCore/NFCDataList.h"

#ifdef NF_USE_ACTOR

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
	};

    EACTOR_MESSAGE_ID eType;
    int nSubMsgID;
	std::string data;
	////////////////////event/////////////////////////////////////////////////
	NFIDENTID self;
	NF_SHARE_PTR<NFAsyncEventList> xActorEventList;//包含同步异步等回调接口
	//////////////////////////////////////////////////////////////////////////
protected:
private:
};

class NFIActor : public Theron::Actor, public NFILogicModule
{
public:

    // Constructor, passes the framework to the baseclass.
	//NFIActor(Theron::Framework &framework, NFIActorManager* pManager, NFIActorManager::EACTOR eActor) : Theron::Actor(framework)
	NFIActor(Theron::Framework &framework, NFIActorManager* pManager) : Theron::Actor(framework)
    {
        m_pActorManager = pManager;
        //meMainActor = eActor;
        // Register the message handler.
        RegisterHandler(this, &NFIActor::Handler);
    }

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from){};

private:
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from)
    {
		//收到消息要处理逻辑
		if (message.eType == NFIActorMessage::EACTOR_EVENT_MSG)
		{
			NFAsyncEventFunc xActorEventFunc;
			bool bRet = message.xActorEventList->First(xActorEventFunc);
			while (bRet)
			{
				std::string strData = message.data;
				xActorEventFunc.xBeginFuncptr->operator()(message.self, message.nSubMsgID, strData);

				bRet = message.xActorEventList->Next(xActorEventFunc);
			}

			Send(message, from);
		}
	}
private:

    NFIActorManager* m_pActorManager;
public:

    NFIActorManager* GetActorManager(){return m_pActorManager;}
};

#endif

#endif