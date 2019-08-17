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

NFGUID NFClanModule::CreateClan(const std::string& strClanName, const std::string& strClanDesc)
{
	NF_SHARE_PTR<NFIObject> xClanObject = m_pKernelModule->CreateObject(NFGUID(), 0, 0, NFrame::Clan::Clan::ThisName(), "", NFDataList());
	if (xClanObject)
	{
		const NFGUID xClanID = xClanObject->Self();
		m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_CreateTime(), NFGetTimeS());
		m_pKernelModule->SetPropertyInt(xClanID, NFrame::Clan::Clan_Level(), 1);
		m_pKernelModule->SetPropertyObject(xClanID, NFrame::Clan::ID(), xClanID);
		m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_Name(), strClanName);
		m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_Desc(), strClanDesc);
		//m_pKernelModule->SetPropertyObject(xClanID, NFrame::Clan::Clan_PresidentID(), self);
		//m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_PresidentName(), "");

		//AddMember(xClanID, self, MEMBER_TYPE::MT_PRESIDENT);

		//m_pLogModule->LogInfo();

		return xClanID;
	}

    return NFGUID();
}

bool NFClanModule::LeaveClan(const NFGUID& xClanID, const NFGUID& self)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord && !self.IsNull())
	{
		int nRow = xMemberRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
		if (nRow >= 0)
		{
			NFIClanModule::MEMBER_TYPE memberType = (MEMBER_TYPE)xMemberRecord->GetInt(nRow, NFrame::Clan::Clan_MemberList::Title);
			if (memberType == NFIClanModule::MEMBER_TYPE::MT_PRESIDENT)
			{
				//find a elder and let him work as the leader
				//how many members now? if the member count is ZERO then we destroy the clan
			}

			xMemberRecord->Remove(nRow);
		}

	}

    return false;
}

bool NFClanModule::PromotionMember(const NFGUID& xClanID, const NFGUID& self, const NFGUID& xMember)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord && !self.IsNull() && !xMember.IsNull())
	{
		NFIClanModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xClanID);
		NFIClanModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xClanID);
		if (eMemberLevelOperater > eMemberLevel)
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

bool NFClanModule::DemotionMember(const NFGUID& xClanID, const NFGUID& self, const NFGUID& xMember)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord && !self.IsNull() && !xMember.IsNull())
	{
		NFIClanModule::MEMBER_TYPE eMemberLevelOperater = CheckPower(self, xClanID);
		NFIClanModule::MEMBER_TYPE eMemberLevel = CheckPower(xMember, xClanID);
		if (eMemberLevelOperater > eMemberLevel)
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

bool NFClanModule::MemberOnline(const NFGUID& xClanID, const NFGUID& self, const int bp)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan::ThisName());
	if (xRecord && !self.IsNull())
	{
		const int nRow = xRecord->FindObject(NFrame::Clan::Clan_MemberList::GUID, self);
		if (nRow >= 0)
		{
			xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::Online, 1);
			xRecord->SetInt(nRow, NFrame::Clan::Clan_MemberList::GameID, 1);

			return true;
		}
	}
		

	return false;
}

bool NFClanModule::MemberOffline(const NFGUID& xClanID, const NFGUID& self, const int bp)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan::ThisName());
	if (xRecord && !self.IsNull())
	{
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
	}
	
    return false;
}

bool NFClanModule::AddMember(const NFGUID& xClanID, const NFGUID& player, const std::string& strPlayerName, const int bp, const MEMBER_TYPE type)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord && !player.IsNull())
	{
		int memberCount = 0;
		for (int i = 0; i < xMemberRecord->GetRows(); ++i)
		{
			if (xMemberRecord->IsUsed(i))
			{
				memberCount++;
			}
		}

		if (memberCount < mnMaxCount)
		{
			NF_SHARE_PTR<NFDataList> xDataList = xMemberRecord->GetInitData();

			xDataList->SetObject(NFrame::Clan::Clan_MemberList::GUID, player);
			xDataList->SetString(NFrame::Clan::Clan_MemberList::Name, strPlayerName);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Level, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Job, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Donation, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Receive, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::BP, bp);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Online, 1);//maybe offline
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Title, type);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::GameID, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::JoinTime, NFGetTimeS());
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::Contribution, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::LastTime, 0);
			xDataList->SetInt(NFrame::Clan::Clan_MemberList::AllContribution, 0);


			xMemberRecord->AddRow(-1, *xDataList);

			if (type == MEMBER_TYPE::MT_PRESIDENT)
			{
				m_pKernelModule->SetPropertyObject(xClanID, NFrame::Clan::Clan_PresidentID(), player);
				m_pKernelModule->SetPropertyString(xClanID, NFrame::Clan::Clan_PresidentName(), strPlayerName);
			}
			//send all members's infomation to this new member
		}
	}

	return false;
}


NFIClanModule::MEMBER_TYPE NFClanModule::CheckPower(const NFGUID& xClanID, const NFGUID& self)
{
	NF_SHARE_PTR<NFIRecord> xMemberRecord = m_pKernelModule->FindRecord(xClanID, NFrame::Clan::Clan_MemberList::ThisName());
	if (xMemberRecord && !self.IsNull())
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

    NFGUID clanID = CreateClan(xMsg.clan_name(), xMsg.clan_desc());
	if (!clanID.IsNull())
	{
		AddMember(clanID, NFINetModule::PBToNF(xMsg.clan_player_id()), xMsg.clan_player_name(), xMsg.clan_player_bp(), NFIClanModule::MT_PRESIDENT);
	}
}

void NFClanModule::OnJoinClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckJoinClan);

	NF_SHARE_PTR<NFIObject> clanObject = m_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.clan_id()));
	if (clanObject)
	{
		AddMember(clanObject->Self(), NFINetModule::PBToNF(xMsg.clan_player_id()), xMsg.clan_player_name(), xMsg.clan_player_bp(), NFIClanModule::MT_MEMBER);
	}
}

void NFClanModule::OnLeaveClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckLeaveClan);
	
	NF_SHARE_PTR<NFIObject> clanObject = m_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.clan_id()));
	if (clanObject)
	{
		LeaveClan(clanObject->Self(), NFINetModule::PBToNF(xMsg.clan_player_id()));
	}
}

void NFClanModule::OnOprClanMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckOprClanMember);

	NF_SHARE_PTR<NFIObject> clanObject = m_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.clan_id()));
	if (clanObject)
	{
		if (xMsg.type() == NFMsg::ReqAckOprClanMember::EGAT_UP)
		{
			PromotionMember(clanObject->Self(), NFINetModule::PBToNF(xMsg.player_id()), NFINetModule::PBToNF(xMsg.member_id()));
		}
		else if (xMsg.type() == NFMsg::ReqAckOprClanMember::EGAT_DOWN)
		{
    		DemotionMember(clanObject->Self(), NFINetModule::PBToNF(xMsg.player_id()), NFINetModule::PBToNF(xMsg.member_id()));
		}
	}
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
	const std::string& strRecord = xEventData.strRecordName;
	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xAddRecordRow.set_record_name(strRecord);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(xEventData.nRow);

		NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, strRecord);
		if (xRecord)
		{
			NFDataList xRowDataList;
			if (xRecord->QueryRow(xEventData.nRow, xRowDataList))
			{
				for (int i = 0; i < xRowDataList.GetCount(); i++)
				{
					switch (xRowDataList.Type(i))
					{
					case TDATA_INT:
					{
						int64_t nValue = xRowDataList.Int(i);

						NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(nValue);
					}
					break;
					case TDATA_FLOAT:
					{
						double fValue = xRowDataList.Float(i);

						NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(fValue);
					}
					break;
					case TDATA_STRING:
					{
						const std::string& str = xRowDataList.String(i);
						NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(str);
					}
					break;
					case TDATA_OBJECT:
					{
						NFGUID identValue = xRowDataList.Object(i);
						NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);

						*pAddData->mutable_data() = NFINetModule::NFToPB(identValue);
					}
					break;
					case TDATA_VECTOR2:
					{
						NFVector2 vPos = xRowDataList.Vector2(i);
						NFMsg::RecordVector2* pAddData = pAddRowData->add_record_vector2_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;
					case TDATA_VECTOR3:
					{
						NFVector3 vPos = xRowDataList.Vector3(i);
						NFMsg::RecordVector3* pAddData = pAddRowData->add_record_vector3_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;

					default:
						break;
					}
				}

				SendMessageToGameServer(self, NFMsg::EGMI_ACK_ADD_ROW, xAddRecordRow);
			}
		}
	}
	break;
	case RECORD_EVENT_DATA::Del:
	{
		NFMsg::ObjectRecordRemove xReoveRecordRow;

		NFMsg::Ident* pIdent = xReoveRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xReoveRecordRow.set_record_name(strRecord);
		xReoveRecordRow.add_remove_row(xEventData.nRow);

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_ADD_ROW, xReoveRecordRow);
	}
	break;
	case RECORD_EVENT_DATA::Swap:
	{

		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

		xSwapRecord.set_origin_record_name(strRecord);
		xSwapRecord.set_target_record_name(strRecord);
		xSwapRecord.set_row_origin(xEventData.nRow);
		xSwapRecord.set_row_target(xEventData.nCol);

		SendMessageToGameServer(self, NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord);
	}
	break;
	case RECORD_EVENT_DATA::Update:
	{
		switch (oldVar.GetType())
		{
		case TDATA_INT:
		{
			NFMsg::ObjectRecordInt xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			int64_t nData = newVar.GetInt();
			recordProperty->set_data(nData);

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged);
		}
		break;

		case TDATA_FLOAT:
		{
			NFMsg::ObjectRecordFloat xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetFloat());

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_FLOAT, xRecordChanged);
		}
		break;
		case TDATA_STRING:
		{
			NFMsg::ObjectRecordString xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetString());

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged);
		}
		break;
		case TDATA_OBJECT:
		{
			NFMsg::ObjectRecordObject xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_OBJECT, xRecordChanged);
		}
		break;
		case TDATA_VECTOR2:
		{
			NFMsg::ObjectRecordVector2 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordVector2* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_VECTOR2, xRecordChanged);
		}
		break;
		case TDATA_VECTOR3:
		{
			NFMsg::ObjectRecordVector3 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordVector3* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());

			SendMessageToGameServer(self, NFMsg::EGMI_ACK_RECORD_VECTOR3, xRecordChanged);
		}
		break;
		default:
			return 0;
			break;
		}
	}
	break;
	case RECORD_EVENT_DATA::Create:
		return 0;
		break;
	case RECORD_EVENT_DATA::Cleared:
	{

	}
	break;
	default:
		break;
	}

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
