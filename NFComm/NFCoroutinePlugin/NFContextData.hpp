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
#include <iostream>
#include <ucontext.h>
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
#ifndef _MSC_VER
	NFContextData(NFICoroutineModule* pCoroutineModule)
	{
		meState = NFContextState::NFCOROUTINE_READY;
		mnOutTime = 60 * 1000;
	}
	virtual ~NFContextData()
	{

	}



    ucontext_t ctx_func;
    ucontext_t ctx_resume;

#endif
	NFContextState GetState() { return meState; };
	void SetState(NFContextState state) { meState = state; };
	int GetOutTime() const { return mnOutTime; }

private:
	std::string m_msg;
	int mnOutTime;
	NFContextState meState;

private:
	NFICoroutineModule* m_coroutinueModule;
};

#endif
