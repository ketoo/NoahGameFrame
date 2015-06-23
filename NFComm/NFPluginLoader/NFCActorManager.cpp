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
#ifdef NF_USE_ACTOR
	m_pFramework = new Theron::Framework(NF_ACTOR_THREAD_COUNT);
#endif
	m_pPluginManager = new NFCPluginManager(this);
}

bool NFCActorManager::Init()
{
#ifdef NF_USE_ACTOR

	m_pMainActor = new NFCActor(*m_pFramework, this);
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
	m_pMainActor = new NFCActor(*m_pFramework, this);
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
	//m_pFramework->Execute();
#endif

	return true;
}

#ifdef NF_USE_ACTOR
// void NFCActorManager::OnReceiveActorData( const void *const data, const uint32_t size, const Theron::Address from)
// {
// 	std::cout << "OnReceiveActorData: " << from.AsString() << std::endl;
// }


bool NFCActorManager::OnRequireActor( const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventFunc)
{
	//¶Ñactor»¹ÊÇÕ»actor
	NFIActor* pActor = new NFCActor(*m_pFramework, this);

	NFIActorMessage xMessage;

	xMessage.eType = NFIActorMessage::EACTOR_EVENT_MSG;
	xMessage.data = strArg;
	xMessage.nSubMsgID = nEventID;
	xMessage.self = objectID;
	xMessage.xActorEventFunc = xActorEventFunc;

	return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
}

void NFCActorManager::Handler( const NFIActorMessage& message, const Theron::Address from )
{

}

NFIPluginManager* NFCActorManager::GetPluginManager()
{
	return m_pPluginManager;
}

#endif