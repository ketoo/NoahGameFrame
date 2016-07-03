#include "NFCActorModule.h"

NFCActorModule::NFCActorModule(NFIPluginManager * p)
{
	srand((unsigned)time(NULL));

	m_pFramework = NF_NEW Theron::Framework(NF_ACTOR_THREAD_COUNT);

}

NFCActorModule::~NFCActorModule()
{
	delete m_pFramework;
	m_pFramework = NULL;
}

bool NFCActorModule::Init()
{
	return false;
}

bool NFCActorModule::Shut()
{
	return false;
}

bool NFCActorModule::BeforeShut()
{
	return false;
}

bool NFCActorModule::AfterInit()
{
	return false;
}

bool NFCActorModule::Execute()
{
	return false;
}

bool NFCActorModule::SendMsgToActor(const int nActorIndex, const NFGUID & objectID, const int nEventID, const std::string & strArg)
{
	return false;
}

int NFCActorModule::RequireActor()
{
	//NF_SHARE_PTR<NFIActor> pActor(NF_NEW NFCActor(*m_pFramework, this));
	//mxActorMap.insert(std::make_pair(pActor->GetAddress().AsInteger(), pActor));

	//return pActor->GetAddress().AsInteger();
	return 0;
}

int NFCActorModule::AddBeginFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR & cb)
{
	return 0;
}

int NFCActorModule::AddEndFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR & cb)
{
	return 0;
}

bool NFCActorModule::InitActorPool(const int nSize)
{
	return false;
}
