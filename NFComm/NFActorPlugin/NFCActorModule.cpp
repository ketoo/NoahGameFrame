// -------------------------------------------------------------------------
//    @FileName			:    NFCActorModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//
// -------------------------------------------------------------------------

#include "NFCActorModule.h"
#include "NFComm/NFCore/NFIComponent.h"

NFCActorModule::NFCActorModule(NFIPluginManager* p)
{
	pPluginManager = p;

    srand((unsigned)time(NULL));

    m_pFramework = NF_NEW Theron::Framework(NF_ACTOR_THREAD_COUNT);

    m_pMainActor = NF_SHARE_PTR<NFIActor>(NF_NEW NFCActor(*m_pFramework, this));
}

NFCActorModule::~NFCActorModule()
{
    m_pMainActor.reset();
    m_pMainActor = nullptr;

    delete m_pFramework;
    m_pFramework = NULL;
}

bool NFCActorModule::Init()
{

    return true;
}

bool NFCActorModule::AfterInit()
{

    return true;
}

bool NFCActorModule::BeforeShut()
{
	mxActorMap.ClearAll();
    return true;
}

bool NFCActorModule::Shut()
{
 
    return true;
}

bool NFCActorModule::Execute()
{
	ExecuteEvent();
    return true;
}


int NFCActorModule::RequireActor()
{
    
    NF_SHARE_PTR<NFIActor> pActor(NF_NEW NFCActor(*m_pFramework, this));
	mxActorMap.AddElement(pActor->GetAddress().AsInteger(), pActor);

    return pActor->GetAddress().AsInteger();
}

NF_SHARE_PTR<NFIActor> NFCActorModule::GetActor(const int nActorIndex)
{
	return mxActorMap.GetElement(nActorIndex);
}

bool NFCActorModule::HandlerEx(const NFIActorMessage & message, const int from)
{
	if (!message.bComponentMsg)
	{
		return mxQueue.Push(message);
	}

	return false;
}

bool NFCActorModule::ExecuteEvent()
{
	NFIActorMessage xMsg;
	bool bRet = false;
	bRet = mxQueue.Pop(xMsg);
	while (bRet)
	{
		if (!xMsg.bComponentMsg)
		{
			xMsg.xEndFuncptr->operator()(xMsg.self, xMsg.nFormActor, xMsg.nMsgID, xMsg.data);
			//Actor can be reused in ActorPool mode, so we don't release it.
			//m_pActorManager->ReleaseActor(xMsg.nFormActor);
		}

		bRet = mxQueue.Pop(xMsg);
	}

	return true;
}

bool NFCActorModule::SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        NFIActorMessage xMessage;

		xMessage.bComponentMsg = true;
        xMessage.data = strArg;
        xMessage.nMsgID = nEventID;
        xMessage.nFormActor = m_pMainActor->GetAddress().AsInteger();
        xMessage.self = objectID;

        return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
    }

    return false;
}

bool NFCActorModule::AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        pActor->AddComponent(pComponent);

        return true;
    }

    return false;
}

bool NFCActorModule::ReleaseActor(const int nActorIndex)
{
	return mxActorMap.RemoveElement(nActorIndex);
}

bool NFCActorModule::AddEndFunc(const int nActorIndex, EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        pActor->AddEndFunc(functorPtr_end);

        return true;
    }

    return false;
}