// -------------------------------------------------------------------------
//    @FileName			:    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCActor::NFCActor(Theron::Framework & framework, NFIActorModule* pModule)
	: NFIActor(framework)
{
	m_pActorModule = pModule;
}

NFCActor::~NFCActor()
{
}

void NFCActor::HandlerEx(const NFIActorMessage& message, const Theron::Address from)
{
    m_pActorModule->HandlerEx(message, from.AsInteger());
}

void NFCActor::AddComponent(NF_SHARE_PTR<NFIComponent> pComponent)
{
	mxComponent.AddElement(pComponent->GetComponentName(), pComponent);
	pComponent->SetActor(this);

	pComponent->Awake();
	pComponent->Init();
	pComponent->AfterInit();
	pComponent->ReadyExecute();
}

bool NFCActor::AddBeginunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor)
{
	if (mxProcessFuntor.GetElement(nSubMsgID))
	{
		return false;
	}

	mxProcessFuntor.AddElement(nSubMsgID, xBeginFunctor);
	return true;
}

bool NFCActor::AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor)
{
	if (mxProcessFuntor.GetElement(nSubMsgID))
	{
		return false;
	}

	if (nSubMsgID >= 0)
	{
		mxProcessFuntor.AddElement(nSubMsgID, xEndFunctor);
	}
	else
	{
		mxDefaultEndProcessFuntor = xEndFunctor;
	}

	return true;
}

void NFCActor::Handler(const NFIActorMessage& message, const Theron::Address from)
{
    std::string strData = message.data;

	ACTOR_PROCESS_FUNCTOR_PTR ptrBegin = mxProcessFuntor.GetElement(message.nMsgID);
	if (ptrBegin != nullptr)
	{
		
		ACTOR_PROCESS_FUNCTOR* pFun = ptrBegin.get();
		pFun->operator()(message.self, message.nFormActor, message.nMsgID, strData);
	}
	else
	{
		for (NF_SHARE_PTR<NFIComponent> pComponent = mxComponent.First(); pComponent != nullptr; pComponent = mxComponent.Next())
		{
			if (pComponent->Enable())
			{
				pComponent->OnASyncEvent(message.self, message.nFormActor, message.nMsgID, strData);
			}
		}
	}
 
    ////////////////////////////////////////////////////////
	// must return message 
    NFIActorMessage xReturnMessage;

	xReturnMessage.bComponentMsg = false;
	xReturnMessage.nMsgID = message.nMsgID;
    xReturnMessage.data = strData;
    xReturnMessage.self = message.self;
    xReturnMessage.nFormActor = this->GetAddress().AsInteger();

	ACTOR_PROCESS_FUNCTOR_PTR ptrEnd = mxEndProcessFuntor.GetElement(message.nMsgID);
	if (ptrEnd != nullptr)
	{
		xReturnMessage.xEndFuncptr = ptrEnd;
	}
	else
	{
		//default end function
		if (mxDefaultEndProcessFuntor != nullptr)
		{
			xReturnMessage.xEndFuncptr = mxDefaultEndProcessFuntor;
		}
	}

    Send(xReturnMessage, from);
}

bool NFCActor::SendMsg(const Theron::Address address, const NFIActorMessage& message)
{
    return Send(message, address);
}
