// -------------------------------------------------------------------------
//    @FileName			:    NFCCoroutineModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCCoroutineModule.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


NFCCoroutineModule::NFCCoroutineModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnIndex = 1;
}

NFCCoroutineModule::~NFCCoroutineModule()
{
}

bool NFCCoroutineModule::Execute()
{

	return true;
}

void NFCCoroutineModule::Schedule()
{
	for (auto it = mxCoroutineMap.begin(); it != mxCoroutineMap.end(); it++)
	{
		switch (it->second->GetState())
		{
		case NFContextState::NFCOROUTINE_READY:
		{
			//start
			//1:synchron--exp:sleep
			//2:asynchron--exp:actor
			StartCoroutine(it->second);

		}
		break;
		case NFContextState::NFCOROUTINE_RUNNING:
		{
			if (it->second->GetOutTime() > (uint64_t)NFGetTime())
			{
				it->second->SetState(NFContextState::NFCOROUTINE_DEAD);
			}
			else
			{
				//check actor msessage
			}
		}
		break;
		case NFContextState::NFCOROUTINE_COMPLETED:
		{
			ResumeCoroutine(it->second);
		}
		break;

		default:
			break;
		}
	}

	for (auto it = mxCoroutineMap.begin(); it != mxCoroutineMap.end();)
	{
		if (it->second->GetState() == NFContextState::NFCOROUTINE_DEAD)
		{
			it = mxCoroutineMap.erase(it);
			continue;
		}

		it++;
	}

	return;
}

void NFCCoroutineModule::StartCoroutine(NF_SHARE_PTR<NFContextData> xContextData)
{
	xContextData->SetState(NFContextState::NFCOROUTINE_RUNNING);
	//swap context to main entre
}

void NFCCoroutineModule::ResumeCoroutine(NF_SHARE_PTR<NFContextData> xContextData)
{
	xContextData->SetState(NFContextState::NFCOROUTINE_DEAD);

	//swap context ---continue execute last address
}

NFINT64 NFCCoroutineModule::CreateNewContextID()
{
	return mnIndex++;
}

bool NFCCoroutineModule::StartCoroutine(TaskFunction const & fn)
{
	NF_SHARE_PTR<NFContextData> xContextData(NF_NEW NFContextData());
	NFINT64 nContextID = CreateNewContextID();
	mxCoroutineMap[nContextID] = xContextData;



#ifndef _MSC_VER
	getcontext(&(xContextData.mxCtx));
#endif

	return true;
}

bool NFCCoroutineModule::Init()
{
	return true;
}

bool NFCCoroutineModule::AfterInit()
{
	return true;
}

bool NFCCoroutineModule::Shut()
{
	return true;
}

bool NFCCoroutineModule::BeforeShut()
{
	return true;
}
