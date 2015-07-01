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
		}
	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
	virtual void RegisterActorComponent(NF_SHARE_PTR<NFIComponent> pComponent);


protected:
	NF_SHARE_PTR<NFIComponent> m_pComponent;
};
#endif