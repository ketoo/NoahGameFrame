// -------------------------------------------------------------------------
//    @FileName         :    NFCGuildEctypeModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCGuildEctypeModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "boost/lexical_cast.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"

#include "NFCGuildEctypeModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCGuildEctypeModule::Init()
{
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>("NFCSceneProcessModule");
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>("NFCCommonConfigModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	
	assert( NULL != m_pSceneProcessModule );
	assert( NULL != m_pGameServerNet_ServerModule );
	assert( NULL != m_pKernelModule );
    assert( NULL != m_pCommonConfigModule );
    assert( NULL != m_pLogModule );

	std::string strConfigPath = pPluginManager->GetConfigPath();

	strConfigPath += "NFDataCfg/Ini/Common/GuildConfig.xml";
	m_pCommonConfigModule->LoadConfig(strConfigPath);

    if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_ReqEnterGuildEctype, this, &NFCGuildEctypeModule::OnApplyEnterGuilEctypeProcess)){ return false; }

	return true;
}


bool NFCGuildEctypeModule::Shut()
{
    return true;
}

bool NFCGuildEctypeModule::Execute()
{
    return true;
}

bool NFCGuildEctypeModule::AfterInit()
{
    m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCGuildEctypeModule::OnGuildClassEvent);
	return true;
}


int NFCGuildEctypeModule::OnGuildClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == NFrame::Guild::ThisName() )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
        {
            CreateGuilEctype(NFGUID(), self);
        } 
        else if ( CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent )
        {
            DestroyGuildEctype(NFGUID(), self);
        }
    }

    return 0;
}

bool NFCGuildEctypeModule::CreateGuilEctype(const NFGUID& self, const NFGUID& xGuild)
{
    if (xGuild.IsNull())
    {
        return false;
    }

    int nEctypeID = m_pCommonConfigModule->GetAttributeInt("GuildEctype", "GuildEctypeInfo", "EctypeID");
    int nNewGroupID = 0;
    if (!m_pSceneProcessModule->ApplyCloneGroup(nEctypeID, nNewGroupID))
    {
        return  false;
    }

    NF_SHARE_PTR<GuildEctypeInfo> pData(NF_NEW GuildEctypeInfo());
    pData->nEctypeID = m_pCommonConfigModule->GetAttributeInt("GuildEctype", "GuildEctypeInfo", "EctypeID");
    pData->nGroupID = nNewGroupID;

    if (!mmGuilEctype.AddElement(xGuild, pData))
    {
        //Realse Clone
        m_pKernelModule->ReleaseGroupScene(nEctypeID, nNewGroupID);
        return false;
    }

    NFCDataList varEntry;
    varEntry << xGuild;
    varEntry << NFINT64(0);
    varEntry << pData->nEctypeID;
    varEntry << pData->nGroupID;
    m_pKernelModule->DoEvent( xGuild, NFED_ON_CLIENT_ENTER_SCENE, varEntry );

    return true;
}

bool NFCGuildEctypeModule::ApplyEnterGuilEctype(const NFGUID& self, const NFGUID& guild)
{
	NF_SHARE_PTR<GuildEctypeInfo> pData	= mmGuilEctype.GetElement(guild);
	if (pData.get())
	{
        NFCDataList varEntry;
        varEntry << self;
        varEntry << NFINT64(0);
        varEntry << pData->nEctypeID;
        varEntry << pData->nGroupID;
        m_pKernelModule->DoEvent( self, NFED_ON_CLIENT_ENTER_SCENE, varEntry );

        return true;
    }

    return false;
}

bool NFCGuildEctypeModule::DestroyGuildEctype( const NFGUID& self, const NFGUID& xGuild )
{
    if (xGuild.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<GuildEctypeInfo> pData = mmGuilEctype.GetElement(xGuild);
    if (pData.get())
    {
        //Realse Clone
        m_pKernelModule->ReleaseGroupScene(pData->nEctypeID, pData->nGroupID);

        mmGuilEctype.RemoveElement(xGuild);
        return true;
    }

    return true;
}

void NFCGuildEctypeModule::OnApplyEnterGuilEctypeProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqEnterGuildEctype);

    const NFGUID& xGuild = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::GuildID());
    if (xGuild.IsNull())
    {
        return ;
    }

    if (!ApplyEnterGuilEctype(nPlayerID, xGuild))
    {
        return;
    }
}
