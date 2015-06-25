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
	srand((unsigned)time(NULL));
#ifdef NF_USE_ACTOR
	m_pFramework = new Theron::Framework(NF_ACTOR_THREAD_COUNT);
#endif
	m_pPluginManager = new NFCPluginManager(this);
}

bool NFCActorManager::Init()
{
#ifdef NF_USE_ACTOR

	m_pMainActor = new NFCActor(*m_pFramework, this);
	for (int i= 0; i < NF_ACTOR_THREAD_COUNT*2; ++i)
	{
		mvActorList.push_back(new NFCActor(*m_pFramework, this));
	}
#endif

	m_pPluginManager->Init();

	return true;
}

bool NFCActorManager::AfterInit()
{
	m_pPluginManager->AfterInit();

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
	delete m_pMainActor;
	m_pMainActor = NULL;

	for (int i= 0; i < NF_ACTOR_THREAD_COUNT*2; ++i)
	{
		NFIActor* pActor = mvActorList[i];
		delete pActor;
	}

	mvActorList.clear();
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
bool NFCActorManager::OnRequireActor( const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventFunc)
{
	//¶Ñactor»¹ÊÇÕ»actor
	int nIndex = rand() % (NF_ACTOR_THREAD_COUNT*2);
	if (nIndex > mvActorList.size() || nIndex < 0)
	{
		return false;
	}

	NFIActor* pActor = mvActorList[nIndex];

	NFIActorMessage xMessage;

	xMessage.eType = NFIActorMessage::EACTOR_EVENT_MSG;
	xMessage.data = strArg;
	xMessage.nSubMsgID = nEventID;
	xMessage.self = objectID;
	xMessage.xActorEventFunc = xActorEventFunc;

	return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
}

#endif

NFIPluginManager* NFCActorManager::GetPluginManager()
{
	return m_pPluginManager;
}