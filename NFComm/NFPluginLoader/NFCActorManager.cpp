// -------------------------------------------------------------------------
//    @FileName      :    NFCActorManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorManager
//
// -------------------------------------------------------------------------

#include "NFCActorManager.h"
#include "NFCPluginManager.h"

NFCActorManager::NFCActorManager()
{
	m_pPluginManager = NULL;

#ifdef NF_USE_ACTOR
	m_pFramework = NULL;
	m_pFramework = new Theron::Framework(NF_ACTOR_THREAD_COUNT);
#endif

	m_pPluginManager = new NFCPluginManager(this);
}

bool NFCActorManager::Init()
{


#ifdef NF_USE_ACTOR
// 
// 	std::vector<NFIActor*>::iterator it = mActorVec.begin();
// 	for (; it != mActorVec.end(); ++it)
// 	{
// 		NFIActor* pActor = *it;
// 
// 		Theron::Receiver receiver;
// 		NFIActorMessage xMessage;
// 		xMessage.eType = NFIActorMessage::EACTOR_INIT;
// 		m_pFramework->Send(xMessage, receiver.GetAddress(), pActor->GetAddress());
// 
// 		receiver.Wait();
// 
//	}

#endif

	m_pPluginManager->Init();

	return true;
}

bool NFCActorManager::AfterInit()
{
	m_pPluginManager->AfterInit();

// #ifdef NF_USE_ACTOR
// 
// 	std::vector<NFIActor*>::iterator it = mActorVec.begin();
// 	for (; it != mActorVec.end(); ++it)
// 	{
// 		NFIActor* pActor = *it;
// 
// 		Theron::Receiver xReceiver;
// 		NFIActorMessage xMessage;
// 		xMessage.eType = NFIActorMessage::EACTOR_AFTER_INIT;
// 		m_pFramework->Send(xMessage, xReceiver.GetAddress(), pActor->GetAddress());
// 
// 		xReceiver.Wait();
// 
//	}
// #endif
	return true;
}

bool NFCActorManager::CheckConfig()
{
	m_pPluginManager->CheckConfig();

#ifdef NF_USE_ACTOR

#endif

    return true;
}

bool NFCActorManager::BeforeShut()
{
#ifdef NF_USE_ACTOR

#endif

	m_pPluginManager->BeforeShut();

	return true;
}

bool NFCActorManager::Shut()
{


#ifdef NF_USE_ACTOR

    delete m_pFramework;
    m_pFramework = NULL;
#endif

	m_pPluginManager->Shut();

	return true;
}

bool NFCActorManager::Execute( const float fLasFrametime, const float fStartedTime )
{
	m_pPluginManager->Execute(fLasFrametime, fStartedTime);

#ifdef NF_USE_ACTOR

#endif

	return true;
}

#ifdef NF_USE_ACTOR
bool NFCActorManager::OnRequireActor( const NFIDENTID& objectID, const int nEventID, const std::string& strArg, NF_SHARE_PTR<NFAsyncEventList> xEventList )
{
	NFIActor* pActor = new NFCActor(*m_pFramework, this);

	Theron::Receiver xReceiver;
	NFIActorMessage xMessage;

	xMessage.eType = NFIActorMessage::EACTOR_EVENT_MSG;
	xMessage.data = strArg;
	xMessage.nSubMsgID = nEventID;
	xMessage.self = objectID;
	xMessage.xEventList = xEventList;

	return m_pFramework->Send(xMessage, xReceiver.GetAddress(), pActor->GetAddress());
}
#endif