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


#include "NFClanModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFClanModule::Init()
{
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pClanRedisModule = pPluginManager->FindModule<NFIClanRedisModule>();
	
	return true;
}

bool NFClanModule::Shut()
{
    return true;
}

bool NFClanModule::Execute()
{
    return true;
}

bool NFClanModule::AfterInit()
{
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_CLAN, this, &NFClanModule::OnCreateClanProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_CLAN, this, &NFClanModule::OnJoinClanProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_CLAN, this, &NFClanModule::OnLeaveClanProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_CLAN, this, &NFClanModule::OnOprClanMemberProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_CLAN, this, &NFClanModule::OnSearchClanProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFClanModule::OnClientChatProcess)) { return false; }

	m_pKernelModule->AddClassCallBack(NFrame::Clan::Clan::ThisName(), this, &NFClanModule::OnObjectClassEvent);

	m_pKernelModule->RegisterClassPropertyEvent(NFrame::Clan::Clan::ThisName(), this, &NFClanModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterClassRecordEvent(NFrame::Clan::Clan::ThisName(), this, &NFClanModule::OnRecordCommonEvent);

    return true;
}

bool NFClanModule::CreateClan(const NFGUID& self, const NFGUID& xClanID, const std::string& strClanName)
{
	NF_SHARE_PTR<NFIObject> xClanObject = m_pKernelModule->CreateObject(xClanID, 0, 0, NFrame::Clan::Clan::ThisName(), "", NFDataList());
	if (xClanObject)
	{
		m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_CreateTime(), 0);
		m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_Level(), 1);
		m_pKernelModule->SetPropertyObject(xClanID, NFrame::Clan::ID(), xClanID);
		m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberMaxCount(), 50);
		m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_Name(), strClanName);
		m_pKernelModule->SetPropertyObject(xClanID, NFrame::Clan::Clan_PresidentID(), self);
		m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_PresidentName(), m_pPlayerRedisModule->GetPropertyString(self, NFrame::Player::Name()));

		AddMember(xClanID, self, MEMBER_TYPE::MT_PRESIDENT);
		
	}

    return false;
}

bool NFClanModule::JoinClan(const NFGUID& self, const NFGUID& xClanID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
		if (nRow < 0)
		{
			return AddMember(xClanID, self, MEMBER_TYPE::MT_MEMBER);
		}
	}

    return false;

}

bool NFClanModule::LeaveClan(const NFGUID& self, const NFGUID& xClanID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
		if (nRow >= 0)
		{
			xMemberRecord->Remove(nRow);

			int nCount = m_pKernelModule->GetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberCount());
			m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberCount(), nCount - 1);
		}

	}

    return false;
}

bool NFClanModule::PromotionMember(const NFGUID& self, const NFGUID& xClanID, const NFGUID& xMember)
{
	NFIClanModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xClanID);
	NFIClanModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xClanID);
	if (eMemberLevelOperater > eMemberLevel)
	{
		NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
		if (xMemberRecord)
		{
			int nSelfRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
			int nMemberRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, xMember);
			if (nSelfRow >= 0 && nMemberRow >= 0)
			{
				int nFutureLevel = eMemberLevel + 1;

				if (nFutureLevel == NFIClanModule::MEMBER_TYPE::MT_VP)
				{
					if (eMemberLevelOperater != NFIClanModule::MEMBER_TYPE::MT_PRESIDENT)
					{
						return false;
					}

					for (int i = 0; i < xMemberRecord->GetRows(); ++i)
					{
						if (xMemberRecord->IsUsed(i))
						{
							int nTitle = xMemberRecord->GetInt(i, NFrame::Clan::Clan_MemberList::Title);
							if (nTitle == NFIClanModule::MEMBER_TYPE::MT_VP)
							{
								xMemberRecord->SetInt(i, NFrame::Clan::Clan_MemberList::Title, NFIClanModule::MEMBER_TYPE::MT_ELDER);
							}
						}
					}

					return xMemberRecord->SetInt(nMemberRow, NFrame::Clan::Clan_MemberList::Title, nFutureLevel);
				}
				else if (nFutureLevel == NFIClanModule::MEMBER_TYPE::MT_PRESIDENT)
				{
					return false;
				}

				return xMemberRecord->SetInt(nMemberRow, NFrame::Clan::Clan_MemberList::Title, nFutureLevel);
			}
		}

	}

    return false;
}

bool NFClanModule::DemotionMember(const NFGUID& self, const NFGUID& xClanID, const NFGUID& xMember)
{
	NFIClanModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xClanID);
	NFIClanModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xClanID);
	if (eMemberLevelOperater > eMemberLevel)
	{
		NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
		if (xMemberRecord)
		{
			int nRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, xMember);
			if (nRow >= 0)
			{
				if (eMemberLevel == MEMBER_TYPE::MT_MEMBER)
				{
					//remove him
					xMemberRecord->Remove(nRow);
					return true;
				}

				eMemberLevel = MEMBER_TYPE(eMemberLevel - 1);
				return xMemberRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::Title, eMemberLevel);
			}
		}

	}

	return false;
}

bool NFClanModule::MemberOnline(const NFGUID& self, const NFGUID& xClan)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xClan, NFrame::Clan::Clan::ThisName());
	const int nRow = xRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
	if (nRow >= 0)
	{
		xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::Online, 1);
		xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::GameID, 1);

		return true;
	}

	return false;
}

bool NFClanModule::MemberOffline(const NFGUID& self, const NFGUID& xClan)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xClan, NFrame::Clan::Clan::ThisName());
	const int nRow = xRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
	if (nRow >= 0)
	{
		xRecord->SetString(nRow, NFrame::Clan::Clan_MemberList::Name, m_pPlayerRedisModule->GetPropertyString(self, NFrame::Player::Name()));
		xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::Level, m_pPlayerRedisModule->GetPropertyInt(self, NFrame::Player::Level()));

		xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::Online, 0);
		xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::GameID, 0);

		NFDateTime xTime = NFDateTime::Now();
		xRecord->SetString(nRow, NFrame::Clan::Clan_MemberList::LastTime, xTime.ToString());

		return true;
	}

    return false;
}

bool NFClanModule::AddMember(const NFGUID & xClanID, const NFGUID & player, const MEMBER_TYPE type)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nCount = m_pKernelModule->GetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberCount());
		int nMaxCount = m_pKernelModule->GetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberMaxCount());
		if (nCount < nMaxCount)
		{
			NF_SHARE_PTR<NFDataList> xDataList = xMemberRecord->GetInitData();

			xDataList->SetObject(NFrame::Clan::Clan_MemberList::GUID, player);
			xDataList->SetString(NFrame::Clan::Clan_MemberList::Name, m_pPlayerRedisModule->GetPropertyString(player,  NFrame::Player::Name()));
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Level, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::Level()));
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Job, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::Job()));
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Donation, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Receive, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::VIP, m_pPlayerRedisModule->GetPropertyInt(player, NFrame::Player::VIPLevel()));
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Online, 1);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Power, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Title, type);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::GameID, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::JoinTime, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Contribution, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::LastTime, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::AllContribution, 0);


			xMemberRecord->AddRow(-1, *xDataList);

			m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_MemeberCount(), nCount + 1);
		}
	}

	return false;
}


NFIClanModule::MEMBER_TYPE NFClanModule::CheckPower(const NFGUID& self, const NFGUID& xClanID)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord)
	{
		int nRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
		if (nRow >= 0)
		{
			return (NFIClanModule::MEMBER_TYPE)xMemberRecord->GetInt(nRow, NFrame::Clan::Clan_MemberList::Title);
		}
	}

    return NFIClanModule::MEMBER_TYPE::MT_MEMBER;
}

void NFClanModule::OnClanOnlineProcess(const NFGUID& xClanID)
{

}

void NFClanModule::OnClanOfflineProcess(const NFGUID& xClanID)
{

}

void NFClanModule::OnCreateClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCreateClan);

    //CreateClan(NFGUID(), xMsg.name xMsg.Clan_name(), );
}

void NFClanModule::OnJoinClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckJoinClan);
}

void NFClanModule::OnLeaveClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckLeaveClan);
}

void NFClanModule::OnOprClanMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckOprClanMember);
}

void NFClanModule::OnSearchClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqSearchClan);

}

void NFClanModule::OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

	NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());

	NF_SHARE_PTR<NFIObject> xGuidObject = m_pKernelModule->GetObject(xTargetID);
	if (xGuidObject)
	{
		//cache the message for the Clan member that off line
		//m_pWorldNet_ServerModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
	}
}

int NFClanModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	return 0;
}

int NFClanModule::OnPropertyCommonEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
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

int NFClanModule::OnRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

void NFClanModule::SendMessageToGameServer(const NFGUID& Clan, const int nMessageID, google::protobuf::Message& msg)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(Clan, NFrame::Clan::Clan_MemberList::ThisName());
	for (int i = 0; i < xRecord->GetRows(); ++i)
	{
		//xRecord->GetInt(i, NFrame::Clan::Clan_MemberList::GameID)
	}

	//m_pWorldNet_ServerModule->SendMsgToGame
}
