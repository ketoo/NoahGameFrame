// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerLogic.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCPlayerLogic
//
// -------------------------------------------------------------------------

#include "stdafx.h"
#include "NFCPlayerLogic.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFCNetLogic.h"
#include "NFCLoginLogic.h"

bool NFCPlayerLogic::Init()
{
	return true;
}

bool NFCPlayerLogic::Shut()
{
	return true;
}

bool NFCPlayerLogic::ReadyExecute()
{
	return true;
}

bool NFCPlayerLogic::Execute()
{


	return true;
}

bool NFCPlayerLogic::AfterInit()
{
	NFCLogicBase::AfterInit();
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_ROLE_LIST, this, &NFCPlayerLogic::OnRoleList);

	
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_ENTRY, this, &NFCPlayerLogic::OnObjectEntry);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_LEAVE, this, &NFCPlayerLogic::OnObjectLeave);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_MOVE, this, &NFCPlayerLogic::OnObjectMove);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_MOVE_IMMUNE, this, &NFCPlayerLogic::OnObjectJump);

	return true;
}

//--------------------------------------------发消息-------------------------------------------------------------
void NFCPlayerLogic::RequireRoleList()
{
	m_RoleList.clear();

	NFMsg::ReqRoleList xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());
	xMsg.set_game_id(g_pLoginLogic->GetServerID());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST, xMsg);
}

void NFCPlayerLogic::RequireCreateRole(string strRoleName, int byCareer, int bySex)
{
	NFMsg::ReqCreateRole xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());	
	xMsg.set_race(0);

	xMsg.set_noob_name(strRoleName);
	xMsg.set_career(byCareer);
	xMsg.set_sex(bySex);
	xMsg.set_game_id(g_pLoginLogic->GetServerID());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE, xMsg);
}

void NFCPlayerLogic::RequireEnterGameServer(int nRoleIndex)
{
	if(nRoleIndex >= m_RoleList.size())
	{
		NFASSERT(0, "out of range", __FILE__, __FUNCTION__);
		return ;
	}

	NFMsg::ReqEnterGameServer xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());	
	xMsg.set_game_id(g_pLoginLogic->GetServerID());

	xMsg.set_name(m_RoleList[nRoleIndex].noob_name());
	*xMsg.mutable_id() = m_RoleList[nRoleIndex].id();
	m_nRoleIndex = nRoleIndex;
	m_RoleGuid = NFINetModule::PBToNF(m_RoleList[nRoleIndex].id());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, xMsg);
}

void NFCPlayerLogic::RequireMove(NFVector3 pos)
{
	NFMsg::ReqAckPlayerMove xMsg;
	*xMsg.mutable_mover() = NFINetModule::NFToPB(m_RoleGuid);
	xMsg.set_movetype(0);
	NFMsg::Vector3 *tPos = xMsg.add_target_pos();
	tPos->set_x((float)pos.X());
	tPos->set_y((float)pos.Y());
	tPos->set_z((float)pos.Z());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_MOVE, xMsg);
}
//--------------------------------------------收消息-------------------------------------------------------------
void NFCPlayerLogic::OnRoleList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckRoleLiteInfoList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	// 目前服务器只有一个角色
	m_RoleList.clear();
	for(int i = 0; i < xMsg.char_data_size(); i++)
	{
		m_RoleList.push_back(xMsg.char_data(i));
	}

	DoEvent(E_PlayerEvent_RoleList, NFDataList());
}

void NFCPlayerLogic::OnObjectEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckPlayerEntryList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	for(int i = 0; i < xMsg.object_list_size(); i++)
	{
		const NFMsg::PlayerEntryInfo info =  xMsg.object_list(i);

		NFDataList var;
		var.Add("X");
		var.AddFloat(info.x());
		var.Add("Y");
		var.AddFloat(info.y());
		var.Add("Z");
		var.AddFloat(info.z());
		g_pKernelModule->CreateObject(NFINetModule::PBToNF(info.object_guid()), info.scene_id(), 0, info.class_id(), info.config_id(), var);
	}
}

void NFCPlayerLogic::OnObjectLeave(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckPlayerLeaveList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	for(int i = 0; i < xMsg.object_list_size(); i++)
	{
		g_pKernelModule->DestroyObject(NFINetModule::PBToNF(xMsg.object_list(i)));
	}
}

// 移动
void NFCPlayerLogic::OnObjectMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckPlayerMove xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
		
	float fMove = g_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "MOVE_SPEED")/10000.0f;
	NFDataList var;
	var.AddObject(NFINetModule::PBToNF(xMsg.mover()));
	var.AddFloat(fMove);
	const NFMsg::Vector3 &pos = xMsg.target_pos(0);
	var.AddVector3(NFVector3(pos.x(), pos.y(), pos.z()));
	DoEvent(E_PlayerEvent_PlayerMove, var);
}

void NFCPlayerLogic::OnObjectJump(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckPlayerMove xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
		
	float fMove = g_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "MOVE_SPEED")/10000.0f;
	NFDataList var;
	var.AddObject(NFINetModule::PBToNF(xMsg.mover()));
	var.AddFloat(fMove);
	const NFMsg::Vector3 &pos = xMsg.target_pos(0);
	var.AddVector3(NFVector3(pos.x(), pos.y(), pos.z()));

	DoEvent(E_PlayerEvent_PlayerJump, var);
}