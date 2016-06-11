// -------------------------------------------------------------------------
//    @FileName         :    NFCMoveModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCMoveModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCTaskModule.h"

bool NFCTaskModule::Init()
{
    return true;
}

bool NFCTaskModule::Shut()
{
    return true;
}

bool NFCTaskModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCTaskModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	assert( NULL != m_pKernelModule );
	assert( NULL != m_pItemConsumeManagerModule );
	assert( NULL != m_pPackModule );
	assert( NULL != m_pElementModule );
	assert(	NULL != m_pLogicClassModule);
	assert(	NULL != m_pPropertyModule);
	assert(	NULL != m_pHeroModule);
	assert(	NULL != m_pCommonConfigModule);
	assert(	NULL != m_pGameServerNet_ServerModule);

	std::string strPlayerPath = pPluginManager->GetConfigPath();
	strPlayerPath += "NFDataCfg/Ini/Common/AwardPackConfig.xml";
	m_pCommonConfigModule->LoadConfig(strPlayerPath);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTaskModule::OnClassObjectEvent );
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFCTaskModule::OnClienAcceptTask)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFCTaskModule::OnClienPushTask)) { return false; }

    return true;
}

int NFCTaskModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
    }

    return 0;
}

void NFCTaskModule::OnClienAcceptTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFCTaskModule::OnClienPushTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}

void NFCTaskModule::OnClienPushCustom(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::cu)
}
