// -------------------------------------------------------------------------
//    @FileName      :    NFCActorManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorManager
//
// -------------------------------------------------------------------------

#include "NFCActorManager.h"
#include "NFCPluginManager.h"
#include "NFComm/NFCore/NFIComponent.h"

NFCActorManager::NFCActorManager()
{
	srand((unsigned)time(NULL));

	m_pFramework = NF_NEW Theron::Framework(NF_ACTOR_THREAD_COUNT);
	pPluginManager = NF_NEW NFCPluginManager(this);

}

bool NFCActorManager::Init()
{
	m_pMainActor = NF_SHARE_PTR<NFIActor>(NF_NEW NFCActor(*m_pFramework, this));

	pPluginManager->Init();

	return true;
}

bool NFCActorManager::AfterInit()
{
	pPluginManager->AfterInit();

	return true;
}

bool NFCActorManager::CheckConfig()
{
	pPluginManager->CheckConfig();

    return true;
}

bool NFCActorManager::BeforeShut()
{
	m_pMainActor.reset();
	m_pMainActor = nullptr;

	pPluginManager->BeforeShut();

	return true;
}

bool NFCActorManager::Shut()
{
    delete m_pFramework;
    m_pFramework = NULL;

	pPluginManager->Shut();

	return true;
}

bool NFCActorManager::Execute()
{
	pPluginManager->Execute();

	return true;
}

int NFCActorManager::RequireActor()
{
	//¶Ñactor
	NF_SHARE_PTR<NFIActor> pActor(NF_NEW NFCActor(*m_pFramework, this));
	mxActorMap.insert(std::make_pair(pActor->GetAddress().AsInteger(), pActor));

	return pActor->GetAddress().AsInteger();
}

NF_SHARE_PTR<NFIActor> NFCActorManager::GetActor(const int nActorIndex)
{
    std::map<int, NF_SHARE_PTR<NFIActor> >::iterator it = mxActorMap.find(nActorIndex);
    if (it != mxActorMap.end())
    {
        return it->second;
    }
        
	return NF_SHARE_PTR<NFIActor>();
}

bool NFCActorManager::SendMsgToActor( const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg)
{
	NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        NFIActorMessage xMessage;

        xMessage.eType = NFIActorMessage::EACTOR_EVENT_MSG;
        xMessage.data = strArg;
        xMessage.nSubMsgID = nEventID;
		xMessage.nFormActor = m_pMainActor->GetAddress().AsInteger();
        xMessage.self = objectID;

        return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
    }

	return false;
}

bool NFCActorManager::AddComponent( const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent )
{
	NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
	if (nullptr != pActor)
	{
		pActor->AddComponent(pComponent);

		return true;
	}

	return false;
}

bool NFCActorManager::ReleaseActor( const int nActorIndex )
{
	std::map<int, NF_SHARE_PTR<NFIActor> >::iterator it = mxActorMap.find(nActorIndex);
	if (it != mxActorMap.end())
	{
		mxActorMap.erase(it);

		return true;
	}

	return false;
}

bool NFCActorManager::AddEndFunc( const int nActorIndex, EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end )
{
	NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
	if (nullptr != pActor)
	{
		pActor->AddEndFunc(functorPtr_end);

		return true;
	}

	return false;
}


