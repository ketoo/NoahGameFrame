// -------------------------------------------------------------------------
//    @FileName         :    NFCGuildEctypeModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCGuildEctypeModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFComm/RapidXML/rapidxml_utils.hpp"

#include "NFCGuildEctypeModule.h"

bool NFCGuildEctypeModule::Init()
{
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	
	assert( NULL != m_pSceneProcessModule );
	assert( NULL != m_pGameServerToWorldModule );
	assert( NULL != m_pKernelModule );
	assert( NULL != m_pCommonConfigModule );

	std::string strConfigPath = pPluginManager->GetConfigPath();

	strConfigPath += "NFDataCfg/Ini/Common/GuildConfig.xml";
	m_pCommonConfigModule->LoadConfig(strConfigPath);

	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_GUILDECTYPEINFO, this, &NFCGuildEctypeModule::OnAckGuildEctypeProcess))
	{
		return false;
	}

	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_SET_GUILDECTYPEINFO, this, &NFCGuildEctypeModule::OnSetGuildEctypeInfoProcess))
	{
		return false;
	}

	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_SET_GUILDECTYPEINFO, this, &NFCGuildEctypeModule::OnSetGuildEctypeInfoProcess)){ return false;}
    
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_OBJECT_ENTRY			, this, &NFCGuildEctypeModule::OnCloneObjectEntryProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_OBJECT_LEAVE			, this, &NFCGuildEctypeModule::OnCloneObjectLeaveProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_OBJECT_PROPERTY_ENTRY	, this, &NFCGuildEctypeModule::OnCloneObjectPropertyEntryProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_OBJECT_RECORD_ENTRY	, this, &NFCGuildEctypeModule::OnCloneObjectRecordEntryProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_PROPERTY_INT			, this, &NFCGuildEctypeModule::OnClonePropertyIntProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_PROPERTY_FLOAT			, this, &NFCGuildEctypeModule::OnClonePropertyFloatProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_PROPERTY_STRING		, this, &NFCGuildEctypeModule::OnClonePropertyStringProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_PROPERTY_DOUBLE		, this, &NFCGuildEctypeModule::OnClonePropertyDoubleProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_PROPERTY_OBJECT		, this, &NFCGuildEctypeModule::OnClonePropertyObjectProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_ADD_ROW				, this, &NFCGuildEctypeModule::OnCloneAddRowProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_REMOVE_ROW				, this, &NFCGuildEctypeModule::OnCloneRemoveRowProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_SWAP_ROW				, this, &NFCGuildEctypeModule::OnCloneSwapRowProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_INT				, this, &NFCGuildEctypeModule::OnCloneRecordIntProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_FLOAT			, this, &NFCGuildEctypeModule::OnCloneRecordFloatProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_DOUBLE			, this, &NFCGuildEctypeModule::OnCloneRecordDoubleProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_STRING			, this, &NFCGuildEctypeModule::OnCloneRecordStringProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_OBJECT			, this, &NFCGuildEctypeModule::OnCloneRecordObjectProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_CLEAR			, this, &NFCGuildEctypeModule::OnCloneRecordClearProcess)){ return false;}
	if (!m_pGameServerToWorldModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_ACK_CLONE_RECORD_SORT			, this, &NFCGuildEctypeModule::OnCloneRecordSortProcess)){ return false;}

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

bool NFCGuildEctypeModule::ApplyEnterGuilEctype(const NFGUID& self, const NFGUID& guild)
{
	NF_SHARE_PTR<GuildEctypeInfo> pData	= mmGuilEctype.GetElement(guild);
	if (pData)
	{
		NFDataList varEntry;
		varEntry << self;
		varEntry << NFINT64(0);
		varEntry << pData->nEctypeID;
		varEntry << pData->nGroupID;
		m_pKernelModule->DoEvent( self, NFED_ON_CLIENT_ENTER_SCENE, varEntry );
	}
	else
	{
		NFMsg::ReqGuildEctypeInfo xMsg;

		*xMsg.mutable_selfid() = NFINetModule::NFToPB(self);
		*xMsg.mutable_guild_id() = NFINetModule::NFToPB(guild);
		m_pGameServerToWorldModule->SendSuitByPB(guild.nData64, NFMsg::EGMI_REQ_GUILDECTYPEINFO, xMsg, 0, self);
	}

	return true;
}

bool NFCGuildEctypeModule::BindGuildEcType(const NFGUID& self, const NFGUID& guild, const int nEctypeID, const int nGroup)
{
	NFMsg::SetGuildEctypeInfo xMsg;
	*xMsg.mutable_selfid() = NFINetModule::NFToPB(self);

	*xMsg.mutable_xectypeinfo()->mutable_guild_id() = NFINetModule::NFToPB(guild);
	xMsg.mutable_xectypeinfo()->set_server_id(pPluginManager->AppID());
	xMsg.mutable_xectypeinfo()->set_ectype_id(nEctypeID);
	xMsg.mutable_xectypeinfo()->set_ectype_group(nGroup);
	m_pGameServerToWorldModule->SendSuitByPB(guild.nData64, NFMsg::EGMI_SET_GUILDECTYPEINFO, xMsg, 0, self);

	return true;
}

void NFCGuildEctypeModule::OnAckGuildEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckGuildEctypeInfo xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen,xData, nPlayerID))
	{
		return;
	}

	NFGUID xGuild = NFINetModule::PBToNF(xData.xectypeinfo().guild_id());
	const int nServerID = xData.xectypeinfo().server_id();
	int nEctypeID = xData.xectypeinfo().ectype_id();
	const int nGroupID  = xData.xectypeinfo().ectype_group();
	
	if (xGuild.IsNull())
	{
		return;
	}
	
	int nSelfServerID = pPluginManager->AppID();
	if (nSelfServerID > 0 && nSelfServerID != nServerID)
	{
		//ÇÐ·þ
		
		return ;
	}

	if (nEctypeID <= 0)
	{
		nEctypeID = m_pCommonConfigModule->GetAttributeInt("GuildEctype", "GuildEctypeInfo", "EctypeID");
	}

	if (nEctypeID <= 0)
	{
		return;
	}

	int nNewGroupID = 0;
	if (!m_pSceneProcessModule->ExitCloneGroup(nEctypeID, nGroupID))
	{
		if (!m_pSceneProcessModule->ApplyCloneGroup(nEctypeID, nNewGroupID))
		{
			return ;
		}
	}

	BindGuildEcType(nPlayerID, xGuild, nEctypeID, nNewGroupID);
}

void NFCGuildEctypeModule::OnSetGuildEctypeInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::SetGuildEctypeInfo xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen,xData, nPlayerID))
	{
		return;
	}

	NFGUID xGuild = NFINetModule::PBToNF(xData.xectypeinfo().guild_id());
	const int nServerID = xData.xectypeinfo().server_id();
	int nEctypeID = xData.xectypeinfo().ectype_id();
	const int nGroupID  = xData.xectypeinfo().ectype_group();

	if (xGuild.IsNull())
	{
		return;
	}

	if (nServerID != pPluginManager->AppID() || nEctypeID <= 0 || nGroupID <= nGroupID)
	{
		return;
	}

	NF_SHARE_PTR<GuildEctypeInfo> pData(NF_NEW GuildEctypeInfo());
	pData->nEctypeID = nEctypeID;
	pData->nGroupID = nGroupID;

	if (!mmGuilEctype.AddElement(xGuild, pData))
	{
		return ;
	}

	if (!nPlayerID.IsNull())
	{
		NFDataList varEntry;
		varEntry << nPlayerID;
		varEntry << NFINT64(0);
		varEntry << pData->nEctypeID;
		varEntry << pData->nGroupID;
		m_pKernelModule->DoEvent( nPlayerID, NFED_ON_CLIENT_ENTER_SCENE, varEntry );
	}
}

void NFCGuildEctypeModule::OnCloneObjectEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nSelf;
	NFMsg::AckPlayerEntryList xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nSelf))
	{
		return;
	}

	for (int i = 0; i < xData.object_list_size(); i++)
	{
		const NFMsg::PlayerEntryInfo& xEntryInfo = xData.object_list(i);
		NFGUID xID = NFINetModule::PBToNF(xEntryInfo.object_guid());
		const int nEctypeID = xEntryInfo.scene_id();
		const int nGroup = xEntryInfo.career_type();
		const std::string& strClass = xEntryInfo.class_id();
		const std::string& strConfigID = xEntryInfo.config_id();
		NFDataList varList;
		m_pKernelModule->CreateObject(xID, nEctypeID, nGroup, strClass, strConfigID, varList);
	}
}

void NFCGuildEctypeModule::OnCloneObjectLeaveProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nSelf;
	NFMsg::AckPlayerLeaveList xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nSelf))
	{
		return;
	}

	for (int i = 0; i < xData.object_list_size(); i++)
	{
		const NFMsg::Ident& xEntryInfo = xData.object_list(i);

		m_pKernelModule->DestroyObject(NFINetModule::PBToNF(xEntryInfo));
	}
}

void NFCGuildEctypeModule::OnCloneObjectPropertyEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nSelf;
	NFMsg::MultiObjectPropertyList xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nSelf))
	{
		return;
	}

	for (int i = 0; i < xData.multi_player_property_size(); i++)
	{
		const NFMsg::ObjectPropertyList& xEntryInfo = xData.multi_player_property(i);

		NFGUID xID = NFINetModule::PBToNF(xEntryInfo.player_id());
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(xID);
		if (nullptr == pObject)
		{
			continue;
		}

		for (int j = 0; j < xEntryInfo.property_int_list_size(); ++j)
		{
			const NFMsg::PropertyInt& xPropertyData = xEntryInfo.property_int_list(i);
			pObject->SetPropertyInt(xPropertyData.property_name(), xPropertyData.data());
		}

		for (int j = 0; j < xEntryInfo.property_float_list_size(); ++j)
		{
			const NFMsg::PropertyFloat& xPropertyData = xEntryInfo.property_float_list(i);
			pObject->SetPropertyFloat(xPropertyData.property_name(), xPropertyData.data());
		}

		for (int j = 0; j < xEntryInfo.property_string_list_size(); ++j)
		{
			const NFMsg::PropertyString& xPropertyData = xEntryInfo.property_string_list(i);
			pObject->SetPropertyString(xPropertyData.property_name(), xPropertyData.data());
		}

		for (int j = 0; j < xEntryInfo.property_object_list_size(); ++j)
		{
			const NFMsg::PropertyObject& xPropertyData = xEntryInfo.property_object_list(i);
			NFGUID xPropertID = NFINetModule::PBToNF(xPropertyData.data());
			pObject->SetPropertyObject(xPropertyData.property_name(), xPropertID);
		}
	}
}

void NFCGuildEctypeModule::OnCloneObjectRecordEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nSelf;
	NFMsg::MultiObjectRecordList xData;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nSelf))
	{
		return;
	}

	for (int i = 0; i < xData.multi_player_record_size(); i++)
	{
		const NFMsg::ObjectRecordList& xEntryInfo = xData.multi_player_record(i);

		NFGUID xID = NFINetModule::PBToNF(xEntryInfo.player_id());
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(xID);
		if (nullptr == pObject)
		{
			continue;
		}

		for (int j = 0; j < xEntryInfo.record_list_size(); j++)
		{
			const NFMsg::ObjectRecordBase& pPrivateRecordBase = xEntryInfo.record_list(i);
			NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(pPrivateRecordBase.record_name());
			if (pRecord)
			{
				OnRecordEnterUnPack(pRecord, pPrivateRecordBase);
			}
		}
	}
}

void NFCGuildEctypeModule::OnClonePropertyIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnClonePropertyFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnClonePropertyStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnClonePropertyDoubleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnClonePropertyObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneAddRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRemoveRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneSwapRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordDoubleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordClearProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnCloneRecordSortProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGuildEctypeModule::OnRecordEnterUnPack(NF_SHARE_PTR<NFIRecord> pRecord, const NFMsg::ObjectRecordBase& xRecordData)
{
	for (int i = 0; i < xRecordData.row_struct_size(); i++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = xRecordData.row_struct(i);
		for (int j = 0; j < xAddRowStruct.record_int_list_size(); ++j)
		{
			const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
			pRecord->SetInt(xPropertyData.row(), xPropertyData.col(), xPropertyData.data());
		}

		for (int j = 0; j < xAddRowStruct.record_float_list_size(); ++j)
		{
			const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
			pRecord->SetFloat(xPropertyData.row(), xPropertyData.col(), xPropertyData.data());
		}

		for (int j = 0; j < xAddRowStruct.record_string_list_size(); ++j)
		{
			const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
			pRecord->SetString(xPropertyData.row(), xPropertyData.col(), xPropertyData.data().c_str());
		}

		for (int j = 0; j < xAddRowStruct.record_object_list_size(); ++j)
		{
			const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
			NFGUID xPropertyID = NFINetModule::PBToNF(xPropertyData.data());
			pRecord->SetObject(xPropertyData.row(), xPropertyData.col(), xPropertyID);
		}
	}
}
