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
#include "NFComm/NFCore/NFIComponent.h"

///////////////////////////////////////////////////
class NFIActor;

struct NFAsyncEventFunc
{
	NFAsyncEventFunc()
	{
		nActorID = -1;
	}

	EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR xBeginFuncptr;
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
	int nActorID;
};

class NFCObjectAsyncEventInfo
	: public NFMapEx<int, NFAsyncEventFunc>
{
};

struct NFClassAsyncEventFunc
{
	CLASS_ASYNC_EVENT_FUNCTOR_PTR xBeginFuncptr;
	CLASS_ASYNC_EVENT_FUNCTOR_PTR xEndFuncptr;
};

struct NFClassAsyncEventList
	: public NFList<NFClassAsyncEventFunc>
{

};

///////////////////////////////////////////////////

class NFIActorManager : public NFILogicModule
{
public:
#ifdef NF_USE_ACTOR
	virtual int OnRequireActor(const NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool OnRequireCPUCycle( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventList) = 0;

#endif

	virtual NFIPluginManager* GetPluginManager() = 0;

};

#endif