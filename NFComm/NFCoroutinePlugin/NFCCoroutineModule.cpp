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
		case NFContextState::NFCOROUTINE_DEAD:
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
	//swap context
}

void NFCCoroutineModule::ResumeCoroutine(NF_SHARE_PTR<NFContextData> xContextData)
{
	xContextData->SetState(NFContextState::NFCOROUTINE_DEAD);

	//swap context
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
