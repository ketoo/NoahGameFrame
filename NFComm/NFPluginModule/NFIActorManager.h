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
template<typename DerivedType , typename BaseType> class TIsDerived
{
public:
	static int AnyFunction(BaseType* base)
	{
		return 1;
	}

	static  char AnyFunction(void* t2)
	{
		return 0;
	}

	enum 
	{
		Result = ( sizeof(int) == sizeof(AnyFunction( (DerivedType*)NULL) ) ), 
	};
};

class NFIActorManager : public NFILogicModule
{
public:
// 	template<typename BaseTypeComponent, typename BaseType>
// 	bool SendMsgToActor(BaseType* pBase, int (BaseType::*handler_end)(const NFIDENTID&, const int, const int, const std::string&), const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg)
// 	{
// 		if (!TIsDerived<BaseTypeComponent, NFIComponent>::Result)
// 		{
// 			//BaseTypeComponent must inherit from NFIComponent;
// 			return 0;
// 		}
// 
// 		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = std::bind(&BaseTypeComponent::OnASyncEvent, pComponent, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
// 		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));
// 
// 		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
// 		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));        
// 
// 		NF_SHARE_PTR<NFAsyncEventFunc> pSyncEventInfo = NF_SHARE_PTR<NFAsyncEventFunc>(NF_NEW NFAsyncEventFunc());
// 
// 		return SendMsgToActor(nActorIndex, objectID, nEventID, strArg, functorPtr_begin, functorPtr_end);
// 	}

	virtual int RequireActor() = 0;
	virtual bool ReleaseActor(const int nActorIndex) = 0;

	virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool SendMsgToActor( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventList) = 0;

	virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex) = 0;

};

#endif