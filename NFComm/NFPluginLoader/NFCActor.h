// -------------------------------------------------------------------------
//    @FileName      :    Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    Actor
//
// -------------------------------------------------------------------------

#ifndef _NFC_ACTOR_H_
#define _NFC_ACTOR_H_

#include <map>
#include <string>
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFCore/NFIComponent.h"

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework &framework, NFIActorManager* pManager) : NFIActor(framework, pManager)
	{

	}

	~NFCActor()
	{
		if (m_pComponent)
		{
			m_pComponent->BeforeShut();
			m_pComponent->Shut();

			m_pComponent.reset();
		}
	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
	virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from);
	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end);

protected:
	NF_SHARE_PTR<NFIComponent> m_pComponent;
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR mxFunctorEndPtr;
};
#endif