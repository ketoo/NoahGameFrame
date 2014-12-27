// -------------------------------------------------------------------------
//    @FileName      :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_MANAGER_H_
#define _NFI_ACTOR_MANAGER_H_

#ifdef NF_USE_ACTOR
#include "Theron/Receiver.h"
#include "Theron/Actor.h"
#include "Theron/Framework.h"
#include "Theron/Detail/Handlers/ReceiverHandler.h"
#endif

#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIActorManager : public NFILogicModule
{
public:
	enum EACTOR
	{
		//模块列表,kernel列表--共用配置后，可以开很多，目前没共用配置，因此需要控制数量，否则配置会占用很多内存
        EACTOR_MAIN,
//         EACTOR_LOGIC1,
// 		EACTOR_LOGIC2,
// 		EACTOR_LOGIC3,
// 		EACTOR_LOGIC4,
		EACTOR_END,
	};

#ifdef NF_USE_ACTOR
	virtual const Theron::Address GetAddress( EACTOR eActor) = 0;
#else
#endif


};

#endif