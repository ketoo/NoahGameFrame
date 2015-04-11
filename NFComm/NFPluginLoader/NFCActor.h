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

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework &framework, NFIActorManager* pManager) : NFIActor(framework, pManager)
	{

	}

};
#endif

#endif