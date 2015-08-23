// -------------------------------------------------------------------------
//    @FileName      :    NFCActorManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorManager
//
// -------------------------------------------------------------------------

#include "NFCActorManager.h"
#include "NFCPluginManager.h"
#include "NFComm/NFCore/NFCMemory.h"
#include "NFComm/NFCore/NFIComponent.h"

NFCActorManager::NFCActorManager()
{
	srand((unsigned)time(NULL));

    m_pFramework = new Theron::Framework(NF_ACTOR_THREAD_COUNT);
	m_pPluginManager = new NFCPluginManager(this);
}

bool NFCActorManager::Init()
{
	m_pMainActor = new NFCActor(*m_pFramework, this);
	for (int i= 0; i < NF_ACTOR_THREAD_COUNT*2; ++i)
	{
		mvActorList.push_back(new NFCActor(*m_pFramework, this));
	}

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

    return true;
}

bool NFCActorManager::BeforeShut()
{
	delete m_pMainActor;
	m_pMainActor = NULL;

	for (int i= 0; i < NF_ACTOR_THREAD_COUNT*2; ++i)
	{
		NFIActor* pActor = mvActorList[i];
		delete pActor;
	}

	mvActorList.clear();

	m_pPluginManager->BeforeShut();

	return true;
}

bool NFCActorManager::Shut()
{
    delete m_pFramework;
    m_pFramework = NULL;

	m_pPluginManager->Shut();

	return true;
}

bool NFCActorManager::Execute( const float fLasFrametime, const float fStartedTime )
{
	m_pPluginManager->Execute(fLasFrametime, fStartedTime);
    
	//m_pFramework->Execute();

	return true;
}

int NFCActorManager::OnRequireActor(NFIComponent* pComponent)
{
	//¶Ñactor
	if (pComponent)
	{
		NF_SHARE_PTR<NFIActor> pActor(NF_NEW NFCActor(*m_pFramework, this));

		pActor->RegisterActorComponent(pComponent);

		mxActorMap.insert(std::make_pair(pActor->GetAddress().AsInteger(), pActor));

		return pActor->GetAddress().AsInteger();
	}
	
	return 0;
}

NFIActor* NFCActorManager::GetActor(const int nActorIndex)
{
    std::map<int, NF_SHARE_PTR<NFIActor>>::iterator it = mxActorMap.find(nActorIndex);
    if (it != mxActorMap.end())
    {
        return it->second.get();
    }

    int nIndex = rand() % (NF_ACTOR_THREAD_COUNT*2);
    if (nIndex > mvActorList.size() || nIndex < 0)
    {
        return NULL;
    }

    return  mvActorList[nIndex];
}

bool NFCActorManager::SendMsgToActor( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventList)
{
	NFIActor* pActor = GetActor(nActorIndex);
    if (pActor)
    {
        NFIActorMessage xMessage;

        xMessage.eType = NFIActorMessage::EACTOR_EVENT_MSG;
        xMessage.data = strArg;
        xMessage.nSubMsgID = nEventID;
		xMessage.nFormActor = m_pMainActor->GetAddress().AsInteger();
        xMessage.self = objectID;
        xMessage.xActorEventFunc = xActorEventList;

        return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
    }

	return false;
}

NFIPluginManager* NFCActorManager::GetPluginManager()
{
	return m_pPluginManager;
}