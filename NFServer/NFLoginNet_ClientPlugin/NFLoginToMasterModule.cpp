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

#include "NFLoginToMasterModule.h"
#include "NFLoginNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFLoginToMasterModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();

    return true;
}

bool NFLoginToMasterModule::Shut()
{
    return true;
}

bool NFLoginToMasterModule::AfterInit()
{
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::ACK_CONNECT_WORLD, this, &NFLoginToMasterModule::OnSelectServerResultProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::STS_NET_INFO, this, &NFLoginToMasterModule::OnWorldInfoProcess);
	
	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_MASTER, this, &NFLoginToMasterModule::OnSocketMSEvent);

	m_pNetClientModule->ExpandBufferSize();

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (serverType == NF_SERVER_TYPES::NF_ST_MASTER)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				//const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				ConnectData xServerData;

				xServerData.nGameID = serverID;
				xServerData.eServerType = (NF_SERVER_TYPES)serverType;
				xServerData.ip = ip;
				xServerData.nPort = nPort;
				xServerData.name = strId;

				m_pNetClientModule->AddServer(xServerData);
			}
		}
    }



    return true;
}

bool NFLoginToMasterModule::BeforeShut()
{

    return false;
}

bool NFLoginToMasterModule::Execute()
{
	ServerReport();
	return true;
}

void NFLoginToMasterModule::Register(NFINet* pNet)
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (serverType == NF_SERVER_TYPES::NF_ST_LOGIN && pPluginManager->GetAppID() == serverID)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                //const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
                const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                NFMsg::ServerInfoReportList xMsg;
                NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

                pData->set_server_id(serverID);
                pData->set_server_name(strId);
                pData->set_server_cur_count(0);
                pData->set_server_ip(ip);
                pData->set_server_port(nPort);
                pData->set_server_max_online(maxConnect);
                pData->set_server_state(NFMsg::EST_NARMAL);
                pData->set_server_type(serverType);

                NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
                if (pServerData)
                {
                    int nTargetID = pServerData->nGameID;
					m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::LTM_LOGIN_REGISTERED, xMsg);

                    m_pLogModule->LogInfo(NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}

void NFLoginToMasterModule::ServerReport()
{
	if (mLastReportTime + 10 > pPluginManager->GetNowTime())
	{
		return;
	}
	mLastReportTime = pPluginManager->GetNowTime();
	std::shared_ptr<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (pPluginManager->GetAppID() == serverID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(serverID);
				reqMsg.set_server_name(strId);
				reqMsg.set_server_cur_count(0);
				reqMsg.set_server_ip(ip);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(maxConnect);
				reqMsg.set_server_state(NFMsg::EST_NARMAL);
				reqMsg.set_server_type(serverType);

				m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::STS_SERVER_REPORT, reqMsg, NFGUID());
			}
		}
	}
}

void NFLoginToMasterModule::OnSelectServerResultProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
    NFGUID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!NFINetModule::ReceivePB( msgID, msg, len, xMsg, nPlayerID))
    {
        return;
    }

    m_pLoginNet_ServerModule->OnSelectWorldResultsProcess(xMsg.world_id(), NFINetModule::PBToNF(xMsg.sender()), xMsg.login_id(), xMsg.account(), xMsg.world_ip(), xMsg.world_port(), xMsg.world_key());
}

void NFLoginToMasterModule::OnSocketMSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_EOF Connection closed", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_ERROR Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_TIMEOUT read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent & NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
        Register(pNet);
    }
}

void NFLoginToMasterModule::OnWorldInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
    NFGUID nPlayerID ;
    NFMsg::ServerInfoReportList xMsg;
    if (!NFINetModule::ReceivePB( msgID, msg, len, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<NFMsg::ServerInfoReport> pServerData = mWorldMap.GetElement(xData.server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
            *pServerData = xData;

            mWorldMap.AddElement(xData.server_id(), pServerData);
        }

    }

    m_pLogModule->LogInfo(NFGUID(0, xMsg.server_list_size()), "", "WorldInfo");
}

NFINetClientModule* NFLoginToMasterModule::GetClusterModule()
{
	return m_pNetClientModule;
}

NFMapEx<int, NFMsg::ServerInfoReport>& NFLoginToMasterModule::GetWorldMap()
{
    return mWorldMap;
}