/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#include "NFDBNet_ServerModule.h"
#include "../NFDBLogicPlugin/NFPlayerRedisModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFDBNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
	
    return true;
}

bool NFDBNet_ServerModule::AfterInit()
{
    m_pNetModule->AddEventCallBack(this, &NFDBNet_ServerModule::OnSocketEvent);
	m_pNetModule->ExpandBufferSize();

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (nServerType == NF_SERVER_TYPES::NF_ST_DB && pPluginManager->GetAppID() == nServerID)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                //const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
                //const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
                if (nRet < 0)
                {
                    std::ostringstream strLog;
                    strLog << "Cannot init server net, Port = " << nPort;
                    m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
                    NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
                    exit(0);
                }
            }
        }
    }

	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_ROLE_LIST, this, &NFDBNet_ServerModule::OnRequireRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_CREATE_ROLE, this, &NFDBNet_ServerModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_DELETE_ROLE, this, &NFDBNet_ServerModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_LOAD_ROLE_DATA, this, &NFDBNet_ServerModule::OnLoadRoleDataProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_SAVE_ROLE_DATA, this, &NFDBNet_ServerModule::OnSaveRoleDataProcess);

    return true;
}

bool NFDBNet_ServerModule::Shut()
{

    return true;
}

bool NFDBNet_ServerModule::Execute()
{

	return true;
}

void NFDBNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent & NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }
}

void NFDBNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
}

void NFDBNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{
}

void NFDBNet_ServerModule::OnRequireRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{

	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID xPlayerID;
	std::string strRoleName;
	if (!m_pPlayerRedisModule->GetRoleInfo(xMsg.account(), strRoleName, xPlayerID))
	{
		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		xAckRoleLiteInfoList.set_account(xMsg.account());
		m_pNetModule->SendMsgPB(NFMsg::ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	xAckRoleLiteInfoList.set_account(xMsg.account());

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

	m_pNetModule->SendMsgPB(NFMsg::ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFDBNet_ServerModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	const std::string& strAccount = xMsg.account();
	const std::string& strName = xMsg.noob_name();
	const int nHomeSceneID = 1;
	NFGUID xID = m_pKernelModule->CreateGUID();

	if (m_pPlayerRedisModule->CreateRole(strAccount, strName, xID, nHomeSceneID))
	{
		m_pPlayerRedisModule->SavePlayerTile(nHomeSceneID, xID, "");

		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		xAckRoleLiteInfoList.set_account(strAccount);

		NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
		pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(xID));
		pData->set_career(xMsg.career());
		pData->set_game_id(pPluginManager->GetAppID());
		pData->set_sex(xMsg.sex());
		pData->set_race(xMsg.race());
		pData->set_noob_name(xMsg.noob_name());
		pData->set_role_level(0);
		pData->set_delete_time(0);
		pData->set_reg_time(0);
		pData->set_last_offline_time(0);
		pData->set_last_offline_ip(0);
		pData->set_view_record("");

		m_pNetModule->SendMsgPB(NFMsg::ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
	}
}

void NFDBNet_ServerModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqDeleteRole xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	xAckRoleLiteInfoList.set_account(xMsg.account());

	m_pNetModule->SendMsgPB(NFMsg::ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFDBNet_ServerModule::OnLoadRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	NFMsg::RoleDataPack xRoleDataxMsg;
	xRoleDataxMsg.mutable_id()->CopyFrom(xMsg.id());

	NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
	pPlayerRedisModule->LoadPlayerData(nRoleID, xRoleDataxMsg);

	m_pNetModule->SendMsgPB(NFMsg::ACK_LOAD_ROLE_DATA, xRoleDataxMsg, nSockIndex);
}

void NFDBNet_ServerModule::OnSaveRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::RoleDataPack xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
	pPlayerRedisModule->SavePlayerData(nRoleID, xMsg);

	//m_pNetModule->SendMsgPB(NFMsg::ACK_LOAD_ROLE_DATA, xMsg, nSockIndex);
}

