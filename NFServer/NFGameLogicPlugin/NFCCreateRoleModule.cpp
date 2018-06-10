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
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pGameToDBModule = pPluginManager->FindModule<NFIGameServerToDBModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	
    return true;
}

bool NFCCreateRoleModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCCreateRoleModule::OnObjectPlayerEvent);

	return true;
}

bool NFCCreateRoleModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME);

	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_ACK_LOAD_ROLE_DATA);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFCCreateRoleModule::OnReqiureRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ROLE_LIST, this, &NFCCreateRoleModule::OnReposeRoleListProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFCCreateRoleModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFCCreateRoleModule::OnDeleteRoleGameProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME, this, &NFCCreateRoleModule::OnClienEnterGameProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_LOAD_ROLE_DATA, this, &NFCCreateRoleModule::OnDBLoadRoleDataProcess);

	return true;
}

void NFCCreateRoleModule::OnReqiureRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, msg, nLen);
}

void NFCCreateRoleModule::OnReposeRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::AckRoleLiteInfoList xData;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xData, nClientID))
	{
		return;
	}

	m_pNetClientModule->SendByServerID(nClientID.GetHead(), nMsgID, msg, nLen);
}

void NFCCreateRoleModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, msg, nLen);
}

void NFCCreateRoleModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, msg, nLen);
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
		if (pGateInfo->eStatus == NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADING)
		{
			return;
		}

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

	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, NFMsg::EGMI_REQ_LOAD_ROLE_DATA, msg, nLen);

}

void NFCCreateRoleModule::OnDBLoadRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::RoleDataPack xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());
	if (m_pKernelModule->GetObject(nRoleID))
	{
		//it should be rebind with proxy's netobject
		m_pKernelModule->DestroyObject(nRoleID);
	}

	mxObjectDataCache[nRoleID] = xMsg;

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = m_pGameServerNet_ServerModule->GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		if (pGateInfo->eStatus == NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADING)
		{
			pGateInfo->eStatus = NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADED;
		}
	}

	NFDataList var;

	var.AddString(NFrame::Player::GateID());
	var.AddInt(pGateInfo->nGateID);

	var.AddString(NFrame::Player::GameID());
	var.AddInt(pPluginManager->GetAppID());

	var.AddString(NFrame::Player::HomeSceneID());
	var.AddInt(1);

	var.AddString(NFrame::Player::SceneID());
	var.AddInt(1);

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, 1, 0, NFrame::Player::ThisName(), "", var);
	if (nullptr == pObject)
	{
		//mRoleBaseData
		//mRoleFDData
		mxObjectDataCache.erase(nRoleID);
		return;
	}

	//get data first then create player
	m_pSceneProcessModule->RequestEnterScene(pObject->Self(), 1, -1, 0, NFDataList());
}

int NFCCreateRoleModule::OnObjectPlayerEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		mxObjectDataCache.RemoveElement(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		mxObjectDataCache.RemoveElement(self);
	}

	return 0;
}

bool NFCCreateRoleModule::Shut()
{
    return true;
}

bool NFCCreateRoleModule::Execute()
{
    return true;
}