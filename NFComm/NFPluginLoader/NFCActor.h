// -------------------------------------------------------------------------
//    @FileName      :    Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    Actor
//
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_H
#define NFC_ACTOR_H

#include <map>
#include <string>
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "../NFCore/NFList.h"

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework &framework, NFIActorManager* pManager) : NFIActor(framework, pManager)
	{

	}

	~NFCActor()
	{
		NF_SHARE_PTR<NFIComponent> pComponent;
		bool bRet = mxComponentList.First(pComponent);
		while (bRet)
		{
			pComponent->BeforeShut();
			pComponent->Shut();

			bRet = mxComponentList.Next(pComponent);
		}

		mxComponentList.ClearAll();
	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
	virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from);
	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end);
	virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message);


protected:
	NFList< NF_SHARE_PTR<NFIComponent> > mxComponentList;
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR mxFunctorEndPtr;
};
#endif