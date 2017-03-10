// -------------------------------------------------------------------------
//    @FileName			:    NFCMailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCMailModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCMailModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCMailModule::Init()
{
	return true;
}

bool NFCMailModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	

	return true;
}

bool NFCMailModule::Shut()
{

	return true;
}

bool NFCMailModule::Execute()
{
	return true;
}
