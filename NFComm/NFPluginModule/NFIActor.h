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
	NF_SHARE_PTR<NFAsyncEventList> xAsyncEventList;//异步事件接口
	NF_SHARE_PTR<NFAsyncEventList> xSyncEventList;//同步回调接口
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
			EVENT_ASYNC_PROCESS_FUNCTOR_PTR cb;// = NF_SHARE_PTR<EVENT_PROCESS_FUNCTOR>(NULL);
			bool bRet = message.xAsyncEventList->First(cb);
			while (bRet)
			{
				cb.get()->operator()(message.self, message.nSubMsgID, message.data);

				bRet = message.xAsyncEventList->Next(cb);
			}

		}
		//处理完毕后发结果到主线程

		//回调到其他线程的EventManager?还是静态函数
//         switch (message.eType)
//         {
//         case NFIActorMessage::EACTOR_INIT:
// 			{
//                 unStartTickTime = ::GetTickCount();
//                 unLastTickTime = unStartTickTime;
// 
// 				Init();
// 				Send(message, from);
// 			}
//             break;
// 
//         case NFIActorMessage::EACTOR_AFTER_INIT:
// 			{
// 				AfterInit();
// 				Send(message, from);
// 			}
//             break;
//         case NFIActorMessage::EACTOR_CHECKCONFIG:
//             {
//                 CheckConfig();
//                 Send(message, from);
//             }
//             break;
// 
//         case NFIActorMessage::EACTOR_EXCUTE:
//             {
//                 unsigned long unNowTickTime = ::GetTickCount();
//                 float fStartedTime = float(unNowTickTime - unStartTickTime) / 1000;
//                 float fLastTime = float(unNowTickTime - unLastTickTime) / 1000;
// 
//                 Execute(fLastTime, fStartedTime);
// 
//                 unLastTickTime = unNowTickTime;
// 
// 				Send(message, from);
//             }
//             break;
// 
//         case NFIActorMessage::EACTOR_BEFORE_SHUT:
// 			{
// 				BeforeShut();
// 				Send(message, from);
// 			}
//             break;
// 
//         case NFIActorMessage::EACTOR_SHUT:
// 			{
// 				Shut();
// 				Send(message, from);
// 			}
//             break;
// 
//         default:
//             HandlerEx(message, from);
//             break;
//         }
    }
private:
private:

    NFIActorManager* m_pActorManager;
   // NFIActorManager::EACTOR meMainActor;
public:

    NFIActorManager* GetActorManager(){return m_pActorManager;}
};

#endif

#endif