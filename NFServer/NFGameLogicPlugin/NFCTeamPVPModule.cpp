// -------------------------------------------------------------------------
//    @FileName         :    NFCTeamPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-06
//    @Module           :    NFCTeamPVPModule
//
// -------------------------------------------------------------------------

#include "NFCTeamPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTeamPVPModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pGSSwitchServerModule = pPluginManager->FindModule<NFIGSSwichServerModule>();

    return true;
}


bool NFCTeamPVPModule::Shut()
{
    return true;
}

bool NFCTeamPVPModule::Execute()
{
    
    return true;
}

bool NFCTeamPVPModule::AfterInit()
{

	
    return true;
}

