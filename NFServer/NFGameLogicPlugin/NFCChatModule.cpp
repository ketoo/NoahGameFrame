// -------------------------------------------------------------------------
//    @FileName         :    NFCChatModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCChatModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCChatModule.h"

bool NFCChatModule::Init()
{
    return true;
}

bool NFCChatModule::Shut()
{
    return true;
}

bool NFCChatModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCChatModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>( "NFCHeroModule" );
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>( "NFCCommonConfigModule" );
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
    m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>("NFCGameServerToWorldModule");

	assert( NULL != m_pKernelModule );
	assert( NULL != m_pItemConsumeManagerModule );
	assert( NULL != m_pPackModule );
	assert( NULL != m_pElementInfoModule );
	assert(	NULL != m_pLogicClassModule);
	assert(	NULL != m_pPropertyModule);
	assert(	NULL != m_pHeroModule);
	assert(	NULL != m_pCommonConfigModule);
    assert(NULL != m_pGameServerNet_ServerModule);
    assert(NULL != m_pGameServerToWorldModule);

	std::string strPlayerPath = pPluginManager->GetConfigPath();
	strPlayerPath += "NFDataCfg/Ini/Common/AwardPackConfig.xml";
	m_pCommonConfigModule->LoadConfig(strPlayerPath);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCChatModule::OnClassObjectEvent );
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCChatModule::OnClienChatProcess)) { return false; }

    return true;
}

int NFCChatModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {

    }
    else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
    {
    }

    return 0;
}

void NFCChatModule::OnClienChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

    switch (xMsg.chat_type())
    {
    case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD:
    {
        NFINetModule* m_pNetModule = m_pGameServerNet_ServerModule->GetNetModule();
        if (m_pNetModule)
        {
            m_pNetModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
        }
    }
    break;
    case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_GUILD:
    {
        NFGUID xGuildID = m_pKernelModule->GetPropertyObject(nPlayerID, "GuildID");
        if (!xGuildID.IsNull())
        {
            NFIClusterClientModule * pClusterClientModule = m_pGameServerToWorldModule->GetClusterClientModule();
            pClusterClientModule->SendBySuit(xGuildID.nData64, nMsgID, msg);
        }
    }
    break;
    case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE:
    case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM:
    {
        if (xMsg.has_target_id())
        {
            NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());
            if (!xTargetID.IsNull())
            {
                NFIClusterClientModule * pClusterClientModule = m_pGameServerToWorldModule->GetClusterClientModule();
                pClusterClientModule->SendBySuit(nPlayerID.nData64, nMsgID, msg);
            }
        }
    }
    break;
    default:
        break;;
    }
}
