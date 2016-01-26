// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCHeroModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCHeroModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCHeroModule::Init()
{
    return true;
}

bool NFCHeroModule::Shut()
{

    return true;
}

bool NFCHeroModule::Execute()
{

    return true;
}

bool NFCHeroModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>( "NFCGameServerNet_ServerModule" );
	
    assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogicClassModule );
	assert( NULL != m_pGameServerNet_ServerModule );

	
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroProcess)){ return false;}



    return true;
}

bool NFCHeroModule::BeforeShut()
{

    return true;
}

bool NFCHeroModule::AddHero( const NFGUID& self, const std::string& strID )
{
    return true;
}

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xID)
{
    return true;
}

void NFCHeroModule::OnSetFightHeroProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	SetFightHero(nPlayerID, xHero);
}