/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFTeamAgentModule.h"

bool NFTeamAgentModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	
    return true;
}

bool NFTeamAgentModule::Shut()
{
    return true;
}

bool NFTeamAgentModule::Execute()
{
    return true;
}

bool NFTeamAgentModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFTeamAgentModule::OnPlayerClassEvent);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_TEAM_LIST, this, &NFTeamAgentModule::OnAckTeammateListProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_TEAM_ADD, this, &NFTeamAgentModule::OnAckPlayerAddProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_TEAM_LEAVE, this, &NFTeamAgentModule::OnAckPlayerLeaveProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_TEAM_INVITE, this, &NFTeamAgentModule::OnAckTeamInviteProcess);

	//from client
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_TEAM_INVITE, this, &NFTeamAgentModule::OnReqSendInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_CANCEL_TEAM_INVITE, this, &NFTeamAgentModule::OnReqCancelInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_TEAM_INVITE, this, &NFTeamAgentModule::OnReqAcceptInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_REJECT_TEAM_INVITE, this, &NFTeamAgentModule::OnReqRejectInviteProcess);


    return true;
}

int NFTeamAgentModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
        //ReqFriendList(self);
	}

	return 0;
}

void NFTeamAgentModule::OnAckTeammateListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckTeamMemberList)
	
	NF_SHARE_PTR<NFIRecord> pTeamMember = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::TeamMemberList::ThisName());
	if (pTeamMember)
	{
		pTeamMember->Clear();

		for (int i = 0; i < xMsg.memberlist_size(); ++i)
		{
			const NFMsg::TeamMemberInfo& memberTeamInfo = xMsg.memberlist(i);

			NF_SHARE_PTR<NFDataList> pData = pTeamMember->GetInitData();
			if (pData)
			{
				const NFGUID memberID = NFINetModule::PBToNF(memberTeamInfo.id());

				pData->SetObject(NFrame::Player::TeamMemberList::GUID, memberID);
				pData->SetString(NFrame::Player::TeamMemberList::Name, memberTeamInfo.name());
				pData->SetInt(NFrame::Player::TeamMemberList::bp, memberTeamInfo.bp());
				pData->SetInt(NFrame::Player::TeamMemberList::Leader, memberTeamInfo.leader());
				pData->SetInt(NFrame::Player::TeamMemberList::Pending, memberTeamInfo.pending());

				pTeamMember->AddRow(-1, *pData);
			}
		}
	}
}

void NFTeamAgentModule::OnAckTeamInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::AckInviteToTeam)
	m_pGameServerNet_ServerModule->SendMsgToGate(nMsgID, std::string(msg, nLen), nPlayerID);
}

void NFTeamAgentModule::OnAckPlayerAddProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::AckTeamMemberAdd)

	for (int i = 0; i < xMsg.member_size(); ++i)
	{
		const NFMsg::TeamMemberInfo& memberTeamInfo = xMsg.member(i);

		NF_SHARE_PTR<NFIRecord> pTeamMember = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::TeamMemberList::ThisName());
		if (pTeamMember)
		{
			const NFGUID memberID = NFINetModule::PBToNF(memberTeamInfo.id());
			const int row = pTeamMember->FindObject(NFrame::Player::TeamMemberList::GUID, memberID);
			if (row >= 0)
			{

			}
			else
			{
				NF_SHARE_PTR<NFDataList> pData = pTeamMember->GetInitData();
				if (pData)
				{
					pData->SetObject(NFrame::Player::TeamMemberList::GUID, memberID);
					pData->SetString(NFrame::Player::TeamMemberList::Name, memberTeamInfo.name());
					pData->SetInt(NFrame::Player::TeamMemberList::bp, memberTeamInfo.bp());
					pData->SetInt(NFrame::Player::TeamMemberList::Leader, memberTeamInfo.leader());
					pData->SetInt(NFrame::Player::TeamMemberList::Pending, memberTeamInfo.pending());

					pTeamMember->AddRow(-1, *pData);
				}
			}
		}
	}
}

void NFTeamAgentModule::OnAckPlayerLeaveProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::AckTeamMemberLeave)
		
	NF_SHARE_PTR<NFIRecord> pTeamMember = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::TeamMemberList::ThisName());
	if (pTeamMember)
	{
		for (int i = 0; i < xMsg.member_size(); ++i)
		{
			const NFGUID memberID = NFINetModule::PBToNF(xMsg.member(i));
			const int row= pTeamMember->FindObject(NFrame::Player::TeamMemberList::GUID, memberID);
			if (row >= 0)
			{
				pTeamMember->Remove(row);
			}
		}
	}
}

void NFTeamAgentModule::OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqInviteToTeam)

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), nMsgID, xMsg, nPlayerID);

}

void NFTeamAgentModule::OnReqCancelInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	/*
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqCancelInviteToTeam)

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), nMsgID, xMsg, nPlayerID);
	*/
}

void NFTeamAgentModule::OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), nMsgID, xMsg, nPlayerID);
}

void NFTeamAgentModule::OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), nMsgID, xMsg, nPlayerID);
}