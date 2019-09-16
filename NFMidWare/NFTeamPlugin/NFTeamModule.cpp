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


#include "NFTeamModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFTeamModule::Init()
{
	return true;
}

bool NFTeamModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pTeamRedisModule = pPluginManager->FindModule<NFITeamRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();


	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_TEAM_INVITE, this, &NFTeamModule::OnReqSendInviteProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_TEAM_INVITE, this, &NFTeamModule::OnReqAcceptInviteProcess)) { return false; }


	return true;
}

void NFTeamModule::OnLine(const NFGUID & self)
{
	NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(self);
	if (pPlayerData)
	{
		if (!pPlayerData->team.IsNull())
		{
			std::vector<NFITeamRedisModule::MemberData> teamMember;
			if (m_pTeamRedisModule->GetMemberList(pPlayerData->team, teamMember))
			{
				for (auto member : teamMember)
				{
					NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pData = m_pWorldNet_ServerModule->GetPlayerData(member.id);
					if (pData)
					{
						if (pData->onLine)
						{
							//broadcast
						}
					}
				}
			}
		}
	}
}

void NFTeamModule::OffLine(const NFGUID & self)
{
	NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(self);
	if (pPlayerData)
	{
		if (!pPlayerData->team.IsNull())
		{
			bool allOffLine = true;
			std::vector<NFITeamRedisModule::MemberData> teamMember;
			if (m_pTeamRedisModule->GetMemberList(pPlayerData->team, teamMember))
			{
				for (auto member : teamMember)
				{
					NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pData = m_pWorldNet_ServerModule->GetPlayerData(member.id);
					if (pData)
					{
						if (pData->onLine)
						{
							allOffLine = false;

							//broadcast
						}
					}
				}

				if (allOffLine)
				{
					m_pTeamRedisModule->DeleteTeam(pPlayerData->team);
				}
			}
		}
	}
}

bool NFTeamModule::Shut()
{

	return true;
}

bool NFTeamModule::Execute()
{
	return true;
}

void NFTeamModule::OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqInviteToTeam)

	const NFGUID strangerID = NFINetModule::PBToNF(xMsg.stranger());
	NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pStrangerData = m_pWorldNet_ServerModule->GetPlayerData(strangerID);
	NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
	if (pPlayerData && pStrangerData)
	{
		if (pStrangerData->team.IsNull())
		{
			bool initial = false;
			if (pPlayerData->team.IsNull())
			{
				initial = true;

				//create team
				const NFGUID teamID = m_pKernelModule->CreateGUID();
				if (m_pTeamRedisModule->CreateTeam(nPlayerID, pPlayerData->name, teamID))
				{
					//add leader to the team
					if (m_pTeamRedisModule->AddToTeam(teamID, nPlayerID, pPlayerData->name))
					{
						pPlayerData->team = teamID;
					}
				}
			}

			if (!pPlayerData->team.IsNull())
			{
				if (m_pTeamRedisModule->SendInvite(pPlayerData->team, strangerID, pStrangerData->name))
				{
					NFMsg::AckInviteToTeam ackInviteToTeam;
					*(ackInviteToTeam.mutable_inviter()) = NFINetModule::NFToPB(nPlayerID);
					ackInviteToTeam.set_name(pPlayerData->name);

					m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_INVITE, ackInviteToTeam, nSockIndex, strangerID);

					//tell all people that we invited a new member
					std::vector<NFITeamRedisModule::MemberData> memberList;
					m_pTeamRedisModule->GetMemberList(pPlayerData->team, memberList);

					for (int i = 0; i < memberList.size(); ++i)
					{
						const NFGUID member = memberList[i].id;

						NFMsg::AckTeamMemberAdd ackInviteToTeam;

						*(ackInviteToTeam.mutable_team_id()) = NFINetModule::NFToPB(pPlayerData->team);
						NFMsg::TeamMemberInfo* pData = ackInviteToTeam.add_member();
						if (pData)
						{
							*(pData->mutable_id()) = NFINetModule::NFToPB(strangerID);
							pData->set_leader(0);
							pData->set_name(pStrangerData->name);
							pData->set_bp(pStrangerData->bp);
							pData->set_pending(1);
						}

						m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_ADD, ackInviteToTeam, nSockIndex, member);
					}
				}

				if (initial)
				{
					//send the team data to the leader
					NFMsg::ReqAckTeamMemberList xReqAckTeamMemberList;
					//send a invite to the stranger   
					std::vector<NFITeamRedisModule::MemberData> memberList;
					m_pTeamRedisModule->GetMemberList(pPlayerData->team, memberList);

					std::vector<NFITeamRedisModule::MemberData> inviteList;
					m_pTeamRedisModule->GetMemberInvitationList(pPlayerData->team, inviteList);

					for (int i = 0; i < memberList.size(); ++i)
					{
						NFMsg::TeamMemberInfo* pData = xReqAckTeamMemberList.add_memberlist();
						if (pData)
						{
							*(pData->mutable_id()) = NFINetModule::NFToPB(memberList[i].id);
							pData->set_leader(1);
							pData->set_name(memberList[i].name);
							pData->set_bp(memberList[i].bp);
							pData->set_pending(0);
						}
					}

					for (int i = 0; i < inviteList.size(); ++i)
					{
						NFMsg::TeamMemberInfo* pData = xReqAckTeamMemberList.add_memberlist();
						if (pData)
						{
							*(pData->mutable_id()) = NFINetModule::NFToPB(inviteList[i].id);
							pData->set_leader(0);
							pData->set_name(inviteList[i].name);
							pData->set_bp(inviteList[i].bp);
							pData->set_pending(1);
						}
					}

					m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_LIST, xReqAckTeamMemberList, nSockIndex, nPlayerID);
				}
			}
		}
	}
}

void NFTeamModule::OnReqCanceltInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{

}

void NFTeamModule::OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckAcceptTeamInvite)
	const NFGUID teamID = NFINetModule::PBToNF(xMsg.team_id());

	NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
	if (pPlayerData)
	{
		std::vector<NFITeamRedisModule::MemberData> inviteList;
		m_pTeamRedisModule->GetMemberInvitationList(teamID, inviteList);

		for (int i = 0; i < inviteList.size(); ++i)
		{
			if (inviteList[i].id == nPlayerID)
			{
				if (m_pTeamRedisModule->DeleteInvite(teamID, nPlayerID) && m_pTeamRedisModule->AddToTeam(teamID, nPlayerID, pPlayerData->name))
				{
					pPlayerData->team = teamID;
					if (m_pTeamRedisModule->AddToTeam(teamID, nPlayerID, pPlayerData->name))
					{
						pPlayerData->team = teamID;
					}

					///////////////////

					NFMsg::ReqAckTeamMemberList xReqAckTeamMemberList;
					std::vector<NFITeamRedisModule::MemberData> memberList;
					m_pTeamRedisModule->GetMemberList(teamID, memberList);

					std::vector<NFITeamRedisModule::MemberData> inviteList;
					m_pTeamRedisModule->GetMemberInvitationList(teamID, inviteList);

					for (int i = 0; i < memberList.size(); ++i)
					{
						NFMsg::TeamMemberInfo* pData = xReqAckTeamMemberList.add_memberlist();
						if (pData)
						{
							*(pData->mutable_id()) = NFINetModule::NFToPB(memberList[i].id);
							pData->set_leader(memberList[i].leader);
							pData->set_name(memberList[i].name);
							pData->set_bp(memberList[i].bp);
							pData->set_pending(0);
						}
					}

					for (int i = 0; i < inviteList.size(); ++i)
					{
						NFMsg::TeamMemberInfo* pData = xReqAckTeamMemberList.add_memberlist();
						if (pData)
						{
							*(pData->mutable_id()) = NFINetModule::NFToPB(inviteList[i].id);
							pData->set_leader(0);
							pData->set_name(inviteList[i].name);
							pData->set_bp(inviteList[i].bp);
							pData->set_pending(1);
						}
					}

					m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_LIST, xReqAckTeamMemberList, nSockIndex, nPlayerID);

					/////tell other people that the team added a new member
					NFMsg::ReqAckTeamMemberList ackTeamMemberList;
					NFMsg::TeamMemberInfo* pData = ackTeamMemberList.add_memberlist();
					if (pData)
					{
						*(pData->mutable_id()) = NFINetModule::NFToPB(pPlayerData->self);
						pData->set_leader(0);
						pData->set_name(pPlayerData->name);
						pData->set_bp(pPlayerData->bp);
						pData->set_pending(0);

						for (int i = 0; i < memberList.size(); ++i)
						{
							const NFGUID member = memberList[i].id;
							if (member != nPlayerID)
							{
								m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_ADD, ackTeamMemberList, nSockIndex, member);
							}
						}
					}
				}

				break;
			}
		}
	}
}

void NFTeamModule::OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
   CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckRejectTeamInvite)

}

void NFTeamModule::OnReqAskReadyProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	//delete all invitation
}
