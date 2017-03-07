// -------------------------------------------------------------------------
//    @FileName			:    NFContextData.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFContextData
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_CONTEXT_DATA_HPP
#define NFC_CONTEXT_DATA_HPP

#include <list>
#include <vector>
#include <set>
#include <stddef.h>
#include <functional>
#include <exception>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFICoroutineModule.h"

enum NFContextState
{
	NFCOROUTINE_DEAD = 0,
	NFCOROUTINE_READY = 1,
	NFCOROUTINE_RUNNING = 2,
	NFCOROUTINE_COMPLETED = 3,
};

class NFContextData
{
public:
	NFContextData(NFICoroutineModule* pCoroutineModule, NFINT64 id, std::size_t stack_size, TaskFunction const& fn)
	{

	}
	virtual ~NFContextData()
	{

	}


private:

	ucontext_t ctx_;

	std::string m_msg;
	NFUINT64 mnOutTime; //yield task out time


private:
	NFICoroutineModule* m_coroutinueModule;
};

#endif