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

#include <Theron/Theron.h>

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
		nFormActor = 0;
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
	int nFormActor;
	std::string data;
	////////////////////event/////////////////////////////////////////////////
	NFIDENTID self;
	//////////////////////////////////////////////////////////////////////////
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
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

	NFIActorManager* GetActorManager(){return m_pActorManager;}

	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end) = 0;
protected:
	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from){};
	virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from){};

private:
    void Handler(const NFIActorMessage& message, const Theron::Address from)
    {
		
		//收到消息要处理逻辑
		if (message.eType == NFIActorMessage::EACTOR_EVENT_MSG)
		{
			HandlerSelf(message, from);

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