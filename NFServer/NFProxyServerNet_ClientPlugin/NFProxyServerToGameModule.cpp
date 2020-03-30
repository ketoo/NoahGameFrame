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
#include "NFProxyServerToGameModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFProxyServerToGameModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pProxyServerNet_ServerModule = pPluginManager->FindModule<NFIProxyServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();

    return true;
}

bool NFProxyServerToGameModule::Shut()
{
    //Final();
    //Clear();
    return true;
}

bool NFProxyServerToGameModule::Execute()
{
	return true;
}

bool NFProxyServerToGameModule::AfterInit()
{
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_GAME, NFMsg::ACK_ENTER_GAME, this, &NFProxyServerToGameModule::OnAckEnterGame);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_GAME, this, &NFProxyServerToGameModule::Transport);

	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_GAME, this, &NFProxyServerToGameModule::OnSocketGSEvent);
	m_pNetClientModule->ExpandBufferSize();

    return true;
}

void NFProxyServerToGameModule::OnSocketGSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
    }
    else  if (eEvent & NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
        Register(pNet);
    }
}

void NFProxyServerToGameModule::Register(NFINet* pNet)
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (nServerType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->GetAppID() == nServerID)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                //const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
                const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                NFMsg::ServerInfoReportList xMsg;
                NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

                pData->set_server_id(nServerID);
                pData->set_server_name(strId);
                pData->set_server_cur_count(0);
                pData->set_server_ip(strIP);
                pData->set_server_port(nPort);
                pData->set_server_max_online(nMaxConnect);
                pData->set_server_state(NFMsg::EST_NARMAL);
                pData->set_server_type(nServerType);

                NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
                if (pServerData)
                {
                    int nTargetID = pServerData->nGameID;
                    m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::PTWG_PROXY_REGISTERED, xMsg);

                    m_pLogModule->LogInfo(NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}

void NFProxyServerToGameModule::OnAckEnterGame(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::AckEventResult xData;
    if (!NFINetModule::ReceivePB( nMsgID, msg, nLen, xData, nPlayerID))
    {
        return;
    }
 
	const NFGUID& xClient = NFINetModule::PBToNF(xData.event_client());
	const NFGUID& xPlayer = NFINetModule::PBToNF(xData.event_object());

	m_pProxyServerNet_ServerModule->EnterGameSuccessEvent(xClient, xPlayer);
	m_pProxyServerNet_ServerModule->Transport(nSockIndex, nMsgID, msg, nLen);
}

void NFProxyServerToGameModule::LogServerInfo(const std::string& strServerInfo)
{
    m_pLogModule->LogInfo(NFGUID(), strServerInfo, "");
}

void NFProxyServerToGameModule::Transport(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pProxyServerNet_ServerModule->Transport(nSockIndex, nMsgID, msg, nLen);
}
