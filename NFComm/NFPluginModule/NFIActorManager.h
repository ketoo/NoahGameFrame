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

///////////////////////////////////////////////////

class NFIActorManager : public NFILogicModule
{
public:
	virtual int OnRequireActor(NFIComponent* pComponent) = 0;
	virtual bool SendMsgToActor( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventList) = 0;

	virtual NFIPluginManager* GetPluginManager() = 0;
	virtual NFIActor* GetActor(const int nActorIndex) = 0;

};

#endif