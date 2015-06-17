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

///////////////////////////////////////////////////

struct NFAsyncEventFunc
{
	EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR xBeginFuncptr;
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
};

struct NFAsyncEventList
	: public NFList<NFAsyncEventFunc>
{

};

class NFCObjectAsyncEventInfo
	: public NFMapEx<int, NFAsyncEventList>
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

	virtual bool OnRequireActor( const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventList> xActorEventList) = 0;
	
#endif


};

#endif