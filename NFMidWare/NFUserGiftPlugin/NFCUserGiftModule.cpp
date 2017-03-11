// -------------------------------------------------------------------------
//    @FileName			:    NFCUserGiftModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCUserGiftModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCUserGiftModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCUserGiftModule::Init()
{
	return true;
}

bool NFCUserGiftModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	

	return true;
}

bool NFCUserGiftModule::Shut()
{

	return true;
}

bool NFCUserGiftModule::Execute()
{
	return true;
}
