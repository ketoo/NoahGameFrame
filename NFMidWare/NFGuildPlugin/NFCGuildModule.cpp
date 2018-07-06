/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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


#include "NFCGuildModule.h"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCGuildModule::Init()
{
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pGuildRedisModule = pPluginManager->FindModule<NFIGuildRedisModule>();
	
	return true;
}

bool NFCGuildModule::Shut()
{
    return true;
}

bool NFCGuildModule::Execute()
{
    return true;
}

bool NFCGuildModule::AfterInit()
{
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCGuildModule::OnCreateGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCGuildModule::OnJoinGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCGuildModule::OnLeaveGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCGuildModule::OnOprGuildMemberProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCGuildModule::OnSearchGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCGuildModule::OnClientChatProcess)) { return false; }

	m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCGuildModule::OnObjectClassEvent);

	m_pKernelModule->RegisterClassPropertyEvent(NFrame::Guild::ThisName(), this, &NFCGuildModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterClassRecordEvent(NFrame::Guild::ThisName(), this, &NFCGuildModule::OnRecordCommonEvent);

    return true;
}

bool NFCGuildModule::CreateGuild(const NFGUID& self, const NFGUID& xGuildID, const std::string& strGuildName)
{
	NF_SHARE_PTR<NFIObject> xGuildObject = m_pKernelModule->CreateObject(xGuildID, 0, 0, NFrame::Guild::ThisName(), "", NFDataList());
	if (xGuildObject)
	{
		m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::Guild_CreateTime(), 0);
		m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::Guild_Level(), 1);
		m_pKernelModule->SetPropertyObject(xGuildID, NFrame::Guild::Guild_ID(), xGuildID);
		m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberMaxCount(), 50);
		m_pKernelModule->SetPropertyString(xGuildID, NFrame::Guild::Guild_Name(), strGuildName);
		m_pKernelModule->SetPropertyObject(xGuildID, NFrame::Guild::Guild_PresidentID(), self);
		m_pKernelModule->SetPropertyString(xGuildID, NFrame::Guild::Guild_PresidentName(), m_pPlayerRedisModule->GetPropertyString(self, NFrame::Player::Name()));

		AddMember(xGuildID, self, MEMBER_TYPE::MT_PRESIDENT);
		
	}

    return false;
}

bool NFCGuildModule::JoinGuild(const NFGUID& self, const NFGUID& xGuildID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, self);
		if (nRow < 0)
		{
			return AddMember(xGuildID, self, MEMBER_TYPE::MT_MEMBER);
		}
	}

    return false;

}

bool NFCGuildModule::LeaveGuild(const NFGUID& self, const NFGUID& xGuildID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, self);
		if (nRow >= 0)
		{
			xMemberRecord->Remove(nRow);

			int nCount = m_pKernelModule->GetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberCount());
			m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberCount(), nCount - 1);
		}

	}

    return false;
}

bool NFCGuildModule::PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{
	NFIGuildModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xGuildID);
	NFIGuildModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xGuildID);
	if (eMemberLevelOperater == MEMBER_TYPE::MT_NONE
		|| eMemberLevel == MEMBER_TYPE::MT_NONE)
	{
		return false;
	}

	if (eMemberLevelOperater > eMemberLevel - 1)
	{
		NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
		if (xMemberRecord)
		{
			int nRow = xMemberRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, xMember);
			if (nRow >= 0)
			{
				return xMemberRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::Title, eMemberLevel + 1);
			}
		}

	}

    return false;
}

bool NFCGuildModule::DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{
	NFIGuildModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xGuildID);
	NFIGuildModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xGuildID);
	if (eMemberLevelOperater == MEMBER_TYPE::MT_NONE
		|| eMemberLevel == MEMBER_TYPE::MT_NONE)
	{
		return false;
	}

	if (eMemberLevelOperater > eMemberLevel)
	{
		NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
		if (xMemberRecord)
		{
			int nRow = xMemberRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, xMember);
			if (nRow >= 0)
			{
				eMemberLevel = MEMBER_TYPE(eMemberLevel - 1);
				if (eMemberLevel == MEMBER_TYPE::MT_NONE)
				{
					eMemberLevel = MEMBER_TYPE::MT_MEMBER;
				}

				return xMemberRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::Title, eMemberLevel);
			}
		}

	}

	return false;
}

bool NFCGuildModule::MemberOnline(const NFGUID& self, const NFGUID& xGuild)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::ThisName());
	const int nRow = xRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, self);
	if (nRow >= 0)
	{
		xRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::Online, 1);
		xRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::GameID, 1);

		return true;
	}

	return false;
}

bool NFCGuildModule::MemberOffline(const NFGUID& self, const NFGUID& xGuild)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::ThisName());
	const int nRow = xRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, self);
	if (nRow >= 0)
	{
		xRecord->SetString(nRow, NFrame::Guild::Guild_MemberList::Name, m_pPlayerRedisModule->GetPropertyString(self, NFrame::Player::Name()));
		xRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::Level, m_pPlayerRedisModule->GetPropertyInt(self, NFrame::Player::Level()));

		xRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::Online, 0);
		xRecord->SetInt(nRow, NFrame::Guild::Guild_MemberList::GameID, 0);

		NFDateTime xTime = NFDateTime::Now();
		xRecord->SetString(nRow, NFrame::Guild::Guild_MemberList::LastTime, xTime.GetAsString());

		return true;
	}

    return false;
}

bool NFCGuildModule::AddMember(const NFGUID & xGuildID, const NFGUID & player, const MEMBER_TYPE type)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nCount = m_pKernelModule->GetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberCount());
		int nMaxCount = m_pKernelModule->GetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberMaxCount());
		if (nCount < nMaxCount)
		{
			NF_SHARE_PTR<NFDataList> xDataList = xMemberRecord->GetInitData();

			xDataList->SetObject(NFrame::Guild::Guild_MemberList::GUID, player);
			xDataList->SetString(NFrame::Guild::Guild_MemberList::Name, m_pPlayerRedisModule->GetPropertyString(player,  NFrame::Player::Name()));
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Level, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::Level()));
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Job, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::Job()));
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Donation, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Receive, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::VIP, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::VIPLevel()));
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Online, 1);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Power, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Title, type);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::GameID, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::JoinTime, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::Contribution, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::LastTime, 0);
			xDataList->SetInt(NFrame::Guild::Guild_MemberList::AllContribution, 0);


			xMemberRecord->AddRow(-1, *xDataList);

			m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::Guild_MemeberCount(), nCount + 1);
		}
	}

	return false;
}


NFIGuildModule::MEMBER_TYPE NFCGuildModule::CheckPower(const NFGUID& self, const NFGUID& xGuildID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::Guild_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Guild::Guild_MemberList::GUID, self);
		if (nRow >= 0)
		{
			return (NFIGuildModule::MEMBER_TYPE)xMemberRecord->GetInt(nRow, NFrame::Guild::Guild_MemberList::Title);
		}
	}

    return NFIGuildModule::MEMBER_TYPE::MT_NONE;
}

void NFCGuildModule::OnGuildOnlineProcess(const NFGUID& xGuildID)
{

}

void NFCGuildModule::OnGuildOfflineProcess(const NFGUID& xGuildID)
{

}

void NFCGuildModule::OnCreateGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);

    //CreateGuild(NFGUID(), xMsg.name xMsg.guild_name(), );
}

void NFCGuildModule::OnJoinGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckJoinGuild);
}

void NFCGuildModule::OnLeaveGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckLeaveGuild);
}

void NFCGuildModule::OnOprGuildMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckOprGuildMember);
}

void NFCGuildModule::OnSearchGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqSearchGuild);

}

void NFCGuildModule::OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

	NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());

	NF_SHARE_PTR<NFIObject> xGuidObject = m_pKernelModule->GetObject(xTargetID);
	if (xGuidObject)
	{
		//cache the message for the guild member that off line
		//m_pWorldNet_ServerModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
	}
}

int NFCGuildModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	return 0;
}

int NFCGuildModule::OnPropertyCommonEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	switch (oldVar.GetType())
	{
	case TDATA_INT:
	{
		NFMsg::ObjectPropertyInt xPropertyInt;
		NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
		pDataInt->set_property_name(strPropertyName);
		pDataInt->set_data(newVar.GetInt());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt);
	}
	break;

	case TDATA_FLOAT:
	{
		NFMsg::ObjectPropertyFloat xPropertyFloat;
		NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
		pDataFloat->set_property_name(strPropertyName);
		pDataFloat->set_data(newVar.GetFloat());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat);
	}
	break;

	case TDATA_STRING:
	{
		NFMsg::ObjectPropertyString xPropertyString;
		NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
		pDataString->set_property_name(strPropertyName);
		pDataString->set_data(newVar.GetString());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString);
	}
	break;

	case TDATA_OBJECT:
	{
		NFMsg::ObjectPropertyObject xPropertyObject;
		NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
		pDataObject->set_property_name(strPropertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject);
	}
	break;
	case TDATA_VECTOR2:
	{
		NFMsg::ObjectPropertyVector2 xPropertyVector2;
		NFMsg::Ident* pIdent = xPropertyVector2.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyVector2* pDataObject = xPropertyVector2.add_property_list();
		pDataObject->set_property_name(strPropertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_VECTOR2, xPropertyVector2);
	}
	break;
	case TDATA_VECTOR3:
	{
		NFMsg::ObjectPropertyVector3 xPropertyVector3;
		NFMsg::Ident* pIdent = xPropertyVector3.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyVector3* pDataObject = xPropertyVector3.add_property_list();
		pDataObject->set_property_name(strPropertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_PROPERTY_VECTOR3, xPropertyVector3);
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFCGuildModule::OnRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

void NFCGuildModule::SendMessageToGameServer(const NFGUID& guild, const int nMessageID, google::protobuf::Message& msg)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(guild, NFrame::Guild::Guild_MemberList::ThisName());
	for (int i = 0; i < xRecord->GetRows(); ++i)
	{
		//xRecord->GetInt(i, NFrame::Guild::Guild_MemberList::GameID)
	}

	//m_pWorldNet_ServerModule->SendMsgToGame
}
