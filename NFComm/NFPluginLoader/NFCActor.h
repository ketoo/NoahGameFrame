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

#ifdef NF_USE_ACTOR

#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFCore/NFIComponent.h"

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework &framework, NFIActorManager* pManager) : NFIActor(framework, pManager)
	{

	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
	virtual void RegisterActorComponent(const std::string& strModuleName, NFIComponent* pComponent);


protected:
	//NFIActorModule*
	NFList<NFIComponent*> mxActorComponent;


};
#endif

#endif