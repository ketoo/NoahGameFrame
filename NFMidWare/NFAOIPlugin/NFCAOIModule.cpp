// -------------------------------------------------------------------------
//    @FileName			:    NFCAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCAOIModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCAOIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCAOIModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();

	return true;
}

bool NFCAOIModule::AfterInit()
{
	
	return true;
}

bool NFCAOIModule::Shut()
{

	return true;
}

bool NFCAOIModule::Execute()
{
	return true;
}
