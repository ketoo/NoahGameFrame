// -------------------------------------------------------------------------
//    @FileName			:    NFIActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActorModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_MODULE_H
#define NFI_ACTOR_MODULE_H

#include "NFILogicModule.h"
#include <Theron/Theron.h>

class NFIActorModule
    : public NFILogicModule
{
public:
	template<typename BaseType>
	int RequireActor(BaseType* pBase,
		int (BaseType::*handler_begin)(const NFGUID&, const int, const int, const std::string&),
		int (BaseType::*handler_end)(const NFGUID&, const int, const int, const std::string&))
	{
		int nActorID = RequireActor();
		if (nActorID >= 0)
		{
			EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = std::bind(handler_begin, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));

			EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

			AddBeginFunc(nActorID, functor_begin);
			AddEndFunc(nActorID, functorPtr_end);

			return nActorID;
		}

		return -1;
	}

	virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg) = 0;

protected:
	virtual int RequireActor() = 0;

	virtual int AddBeginFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb) = 0;
	virtual int AddEndFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb) = 0;
};

#endif