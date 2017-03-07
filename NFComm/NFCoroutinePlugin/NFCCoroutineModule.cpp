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
