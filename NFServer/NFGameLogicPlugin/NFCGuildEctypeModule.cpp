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

bool NFCGuildEctypeModule::Init()
{
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>("NFCSceneProcessModule");
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>("NFCGameServerToWorldModule");
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>("NFCCommonConfigModule");
	
	assert( NULL != m_pSceneProcessModule );
	assert( NULL != m_pGameServerToWorldModule );
	assert( NULL != m_pKernelModule );
	assert( NULL != m_pCommonConfigModule );

	std::string strConfigPath = pPluginManager->GetConfigPath();

	strConfigPath += "NFDataCfg/Ini/Common/GuildConfig.xml";
	m_pCommonConfigModule->LoadConfig(strConfigPath);

    
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

	return true;
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