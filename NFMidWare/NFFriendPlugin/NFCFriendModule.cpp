// -------------------------------------------------------------------------
//    @FileName			:    NFCFriendModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCFriendModule
//    @Desc             :

#include "NFCFriendModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCFriendModule::Init()
{
	return true;
}

bool NFCFriendModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	

	return true;
}

bool NFCFriendModule::Shut()
{

	return true;
}

bool NFCFriendModule::Execute()
{
	return true;
}
