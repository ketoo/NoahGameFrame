// -------------------------------------------------------------------------
//    @FileName			:    NFCChatModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCChatModule
//    @Desc             :

#include "NFCChatModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCChatModule::Init()
{
	return true;
}

bool NFCChatModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	

	return true;
}

bool NFCChatModule::Shut()
{

	return true;
}

bool NFCChatModule::Execute()
{
	return true;
}
