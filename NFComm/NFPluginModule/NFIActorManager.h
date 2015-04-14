// -------------------------------------------------------------------------
//    @FileName      :    NFIActorManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActorManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_MANAGER_H_
#define _NFI_ACTOR_MANAGER_H_

#include "NFILogicModule.h"
#include "NFIEventProcessModule.h"

class NFIActorManager : public NFILogicModule
{
public:
#ifdef NF_USE_ACTOR

	virtual bool OnRequireActor( const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventList> xAsyncEventList, const NF_SHARE_PTR<NFAsyncEventList> xSyncEventList) = 0;
	
#endif


};

#endif