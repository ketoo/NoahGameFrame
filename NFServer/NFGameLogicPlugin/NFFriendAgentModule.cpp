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

#include "NFFriendAgentModule.h"

bool NFFriendAgentModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFFriendAgentModule::Shut()
{
    return true;
}

bool NFFriendAgentModule::Execute()
{
    return true;
}

bool NFFriendAgentModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFFriendAgentModule::OnPlayerClassEvent);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_REQ_ACK_FRIEND_LIST, this, &NFFriendAgentModule::OnAckFriendListProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_FRIEND_DELETE, this, &NFFriendAgentModule::OnAckAddFriendProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_FRIEND_ADD, this, &NFFriendAgentModule::OnAckAddFriendProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_INVITE_ADD, this, &NFFriendAgentModule::OnAckReceivedInviteProcess);

    //from client
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_INVITE, this, &NFFriendAgentModule::OnReqSendInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, this, &NFFriendAgentModule::OnReqAcceptInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_REJECT_INVITE, this, &NFFriendAgentModule::OnReqRejectInviteProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_IGNORE_INVITE, this, &NFFriendAgentModule::OnReqIgnoreInviteProcess);
    //from client
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_BLOCK_PLAYER, this, &NFFriendAgentModule::OnReqBlockPlayerProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_UNBLOCK_PLAYER, this, &NFFriendAgentModule::OnReqUnBlockPlayerProcess);

    return true;
}

int NFFriendAgentModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
        ReqFriendList(self);
	}

	return 0;
}    

void NFFriendAgentModule::ReqFriendList(const NFGUID & self)
{
    NFMsg::ReqAckFriendList xMsg;
    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, self.GetData(), NFMsg::EGMI_REQ_ACK_FRIEND_LIST, xMsg, self);
}

void NFFriendAgentModule::OnAckFriendListProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
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
            NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
            if (dataList)
            {
                dataList->SetObject(NFrame::Player::FriendList::GUID, NFINetModule::PBToNF(friendData.id()));
                dataList->SetString(NFrame::Player::FriendList::Name, friendData.name());

                pRecord->AddRow(-1, *dataList);
            }
        }
    }

    for (int i = 0; i < xMsg.invitelist_size(); ++i)
    {
        const NFMsg::FriendData& friendData = xMsg.invitelist(i);

        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendInviteList::ThisName());
        if (pRecord)
        {
            NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
            if (dataList)
            {
                dataList->SetObject(NFrame::Player::FriendInviteList::GUID, NFINetModule::PBToNF(friendData.id()));
                dataList->SetString(NFrame::Player::FriendInviteList::Name, friendData.name());

                pRecord->AddRow(-1, *dataList);
            }
        }
    }
}

void NFFriendAgentModule::OnAckAddFriendProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
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
            NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
            if (dataList)
            {
				const NFGUID id = NFINetModule::PBToNF(friendData.id());
                dataList->SetObject(NFrame::Player::FriendList::GUID, id);
                dataList->SetString(NFrame::Player::FriendList::Name, friendData.name());

                pRecord->AddRow(-1, *dataList);

				NF_SHARE_PTR<NFIRecord> pSendRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendSentList::ThisName());
				if (pSendRecord)
				{
					int row = pSendRecord->FindObject(NFrame::Player::FriendSentList::GUID, id);
					if (row >= 0)
					{
						pSendRecord->Remove(row);
					}
				}
            }
        }
    }
}

void NFFriendAgentModule::OnAckDeleteFriendProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
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

void NFFriendAgentModule::OnAckReceivedInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from world
    CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)

    //should batch
    for (int i = 0; i < xMsg.invitelist_size(); ++i)
    {
        const NFMsg::FriendData& friendData = xMsg.invitelist(i);

        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendInviteList::ThisName());
        if (pRecord)
        {
            NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
            if (dataList)
            {
                dataList->SetObject(NFrame::Player::FriendInviteList::GUID, NFINetModule::PBToNF(friendData.id()));
                dataList->SetString(NFrame::Player::FriendInviteList::Name, friendData.name());

                pRecord->AddRow(-1, *dataList);
            }
        }
    }
}

void NFFriendAgentModule::OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckSendInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendSentList::ThisName());
    if (pRecord)
    {
		const int row = pRecord->FindObject(NFrame::Player::FriendSentList::GUID, NFINetModule::PBToNF(xMsg.id()));
		if (row < 0)
		{
			NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
			if (dataList)
			{
				dataList->SetObject(NFrame::Player::FriendSentList::GUID, NFINetModule::PBToNF(xMsg.id()));
				dataList->SetString(NFrame::Player::FriendSentList::Name, xMsg.name());

				pRecord->AddRow(-1, *dataList);
			}

			m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_SEND_INVITE, xMsg, nPlayerID);
		}
    }
}

void NFFriendAgentModule::OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendInviteList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::FriendInviteList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, xMsg, nPlayerID);
}

void NFFriendAgentModule::OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendInviteList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::FriendInviteList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_REJECT_INVITE, xMsg, nPlayerID);

}

void NFFriendAgentModule::OnReqIgnoreInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckIgnoreInvite)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::FriendInviteList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::FriendInviteList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_IGNORE_INVITE, xMsg, nPlayerID);

}

void NFFriendAgentModule::OnReqBlockPlayerProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckBlockPlayer)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BlockList::ThisName());
    if (pRecord)
    {
        NF_SHARE_PTR<NFDataList> dataList = pRecord->GetInitData();
        if (dataList)
        {
            dataList->SetObject(NFrame::Player::BlockList::GUID, NFINetModule::PBToNF(xMsg.id()));

            pRecord->AddRow(-1, *dataList);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_BLOCK_PLAYER, xMsg, nPlayerID);

}

void NFFriendAgentModule::OnReqUnBlockPlayerProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    //come from client
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckBlockPlayer)

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BlockList::ThisName());
    if (pRecord)
    {
        int row = pRecord->FindObject(NFrame::Player::BlockList::GUID, NFINetModule::PBToNF(xMsg.id()));
        if (row >= 0)
        {
            pRecord->Remove(row);
        }
    }

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.GetData(), NFMsg::EGMI_REQ_ACK_UNBLOCK_PLAYER, xMsg, nPlayerID);
}