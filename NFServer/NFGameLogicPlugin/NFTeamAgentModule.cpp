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
	/*
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_FRIEND_DELETE, this, &NFTeamAgentModule::OnAckCreateTeamProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_FRIEND_ADD, this, &NFTeamAgentModule::OnAckLeaveTeamProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_INVITE_ADD, this, &NFTeamAgentModule::OnAckReceivedInviteProcess);

	//from client
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_INVITE, this, &NFTeamAgentModule::OnReqSendInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, this, &NFTeamAgentModule::OnReqAcceptInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_REJECT_INVITE, this, &NFTeamAgentModule::OnReqRejectInviteProcess);

	*/

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


void NFTeamAgentModule::OnAckCreateTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from world
    CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)

}

void NFTeamAgentModule::OnAckLeaveTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from world
    CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)

    //should batch
    for (int i = 0; i < xMsg.friendlist_size(); ++i)
    {
        const NFMsg::FriendData& friendData = xMsg.friendlist(i);

        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendList::ThisName());
        if (pRecord)
        {
            int row = pRecord->FindObject(NFrame::Player::FriendList::GUID, NFINetModule::PBToNF(friendData.id()));
            if (row >= 0)
            {
                pRecord->Remove(row);
            }
        }
    }
}

void NFTeamAgentModule::OnAckReceivedInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from world
    CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)

    //should batch
    for (int i = 0; i < xMsg.invitelist_size(); ++i)
    {
        const NFMsg::FriendData& friendData = xMsg.invitelist(i);

        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::InviteList::ThisName());
        if (pRecord)
        {
            NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
            if (dataList)
            {
                dataList->SetObject(NFrame::Player::InviteList::GUID, NFINetModule::PBToNF(friendData.id()));
                dataList->SetString(NFrame::Player::InviteList::Name, friendData.name());

                pRecord->AddRow(-1, *dataList);
            }
        }
    }
}

void NFTeamAgentModule::OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckSendInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::SentList::ThisName());
    if (pRecord)
    {
		const int row = pRecord->FindObject(NFrame::Player::SentList::GUID, NFINetModule::PBToNF(xMsg.id()));
		if (row < 0)
		{
			NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
			if (dataList)
			{
				dataList->SetObject(NFrame::Player::SentList::GUID, NFINetModule::PBToNF(xMsg.id()));
				dataList->SetString(NFrame::Player::SentList::Name, xMsg.name());

				pRecord->AddRow(-1, *dataList);
			}

			m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_SEND_INVITE, xMsg, nPlayerID);
		}
    }
}

void NFTeamAgentModule::OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::InviteList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::InviteList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, xMsg, nPlayerID);
}

void NFTeamAgentModule::OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::InviteList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::InviteList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_REJECT_INVITE, xMsg, nPlayerID);

}