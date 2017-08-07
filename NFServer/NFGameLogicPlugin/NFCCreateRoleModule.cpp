// -------------------------------------------------------------------------
//    @FileName				:    NFCCreateRoleModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-03-29
//    @Module               :    NFCCreateRoleModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCCreateRoleModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCCreateRoleModule::Init()
{
	m_pPVPModule = pPluginManager->FindModule<NFIPVPModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	
    return true;
}

bool NFCCreateRoleModule::AfterInit()
{

	return true;
}

void NFCCreateRoleModule::OnReqiureRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID xPlayerID;
	std::string strRoleName;
	if (!m_pAccountRedisModule->GetRoleInfo(xMsg.account(), strRoleName, xPlayerID))
	{
		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
		return;
	}

	//NF_SHARE_PTR<NFIPropertyManager> xPlayerProperty = m_pPlayerRedisModule->LoadPlayerCacheProperty(xPlayerID);
	//if (xPlayerProperty)
	{
		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
		pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(xPlayerID));
		pData->set_game_id(pPluginManager->GetAppID());
		pData->set_career(0);
		pData->set_sex(0);
		pData->set_race(0);
		pData->set_noob_name(strRoleName);
		pData->set_role_level(0);
		pData->set_delete_time(0);
		pData->set_reg_time(0);
		pData->set_last_offline_time(0);
		pData->set_last_offline_ip(0);
		pData->set_view_record("");

		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
	}
	//else
	//{
	//	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	//	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
	//}
}

void NFCCreateRoleModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	const std::string& strAccount = xMsg.account();
	const std::string& strName = xMsg.noob_name();

	NFGUID xID = m_pKernelModule->CreateGUID();
	if (m_pAccountRedisModule->CreateRole(strAccount, strName, xID))
	{
		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
		pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(xID));
		pData->set_career(xMsg.career());
		pData->set_game_id(pPluginManager->GetAppID());
		pData->set_sex(xMsg.sex());
		pData->set_race(xMsg.race());
		pData->set_noob_name(xMsg.noob_name());
		pData->set_role_level(1);
		pData->set_delete_time(0);
		pData->set_reg_time(0);
		pData->set_last_offline_time(0);
		pData->set_last_offline_ip(0);
		pData->set_view_record("");

		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
	}
}

void NFCCreateRoleModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}
		
	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nClientID);
}

void NFCCreateRoleModule::OnClienEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	if (m_pKernelModule->GetObject(nRoleID))
	{
		//it should be rebind with proxy's netobject
		m_pKernelModule->DestroyObject(nRoleID);
	}

	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = m_pGameServerNet_ServerModule->GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		m_pGameServerNet_ServerModule->RemovePlayerGateInfo(nRoleID);
	}

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> pGateServerinfo = m_pGameServerNet_ServerModule->GetGateServerInfoBySockIndex(nSockIndex);
	if (nullptr == pGateServerinfo)
	{
		return;
	}

	int nGateID = -1;
	if (pGateServerinfo->xServerData.pData)
	{
		nGateID = pGateServerinfo->xServerData.pData->server_id();
	}

	if (nGateID < 0)
	{
		return;
	}

	if (!m_pGameServerNet_ServerModule->AddPlayerGateInfo(nRoleID, nClientID, nGateID))
	{
		return;
	}

	if (m_pPlayerRedisModule->LoadPlayerData(nRoleID))
	{
		int nHomeSceneID = m_pPlayerRedisModule->GetPlayerHomeSceneID(nRoleID);
		if (nHomeSceneID <= 0)
		{
			nHomeSceneID = m_pPVPModule->RandomTileScene();
		}

		NFDataList var;
		var.AddString(NFrame::Player::Name());
		var.AddString(xMsg.name());

		var.AddString(NFrame::Player::GateID());
		var.AddInt(nGateID);

		var.AddString(NFrame::Player::GameID());
		var.AddInt(pPluginManager->GetAppID());

		var.AddString(NFrame::Player::HomeSceneID());
		var.AddInt(nHomeSceneID);

		var.AddString(NFrame::Player::ViewOppnent());
		var.AddObject(nRoleID);

		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nHomeSceneID, 0, NFrame::Player::ThisName(), "", var);
		if (nullptr == pObject)
		{
			//mRoleBaseData
			//mRoleFDData
			return;
		}

		//get data first then create player
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), nHomeSceneID, -1, 0, NFDataList());
	}
}

bool NFCCreateRoleModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFCCreateRoleModule::OnReqiureRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFCCreateRoleModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFCCreateRoleModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME, this, &NFCCreateRoleModule::OnClienEnterGameProcess);

	return true;
}

bool NFCCreateRoleModule::Shut()
{
    return true;
}

bool NFCCreateRoleModule::Execute()
{
    return true;
}