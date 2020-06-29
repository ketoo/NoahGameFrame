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
#include "NFWorldNet_ServerModule.h"
#include "NFWorldNet_ServerPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFWorldNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
	
    return true;
}

bool NFWorldNet_ServerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_REGISTERED, this, &NFWorldNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_UNREGISTERED, this, &NFWorldNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_REFRESH, this, &NFWorldNet_ServerModule::OnRefreshProxyServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::GTW_GAME_REGISTERED, this, &NFWorldNet_ServerModule::OnGameServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::GTW_GAME_UNREGISTERED, this, &NFWorldNet_ServerModule::OnGameServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::GTW_GAME_REFRESH, this, &NFWorldNet_ServerModule::OnRefreshGameServerInfoProcess);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::DTW_DB_REGISTERED, this, &NFWorldNet_ServerModule::OnDBServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::DTW_DB_UNREGISTERED, this, &NFWorldNet_ServerModule::OnDBServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::DTW_DB_REFRESH, this, &NFWorldNet_ServerModule::OnRefreshDBServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_ONLINE_NOTIFY, this, &NFWorldNet_ServerModule::OnOnlineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_OFFLINE_NOTIFY, this, &NFWorldNet_ServerModule::OnOfflineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::STS_SERVER_REPORT, this, &NFWorldNet_ServerModule::OnTransmitServerReport);

    m_pNetModule->AddEventCallBack(this, &NFWorldNet_ServerModule::OnSocketEvent);
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
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && pPluginManager->GetAppID() == nServerID)
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

    return true;
}

void NFWorldNet_ServerModule::OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			return;
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		NFGUID nPlayerID;
		NFMsg::ServerInfoReportList xMsg;
		if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
		{
			return;
		}

		for (int i = 0; i < xMsg.server_list_size(); ++i)
		{
			const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
			const int nAreaID = m_pElementModule->GetPropertyInt(xData.server_name(), NFrame::Server::Area());
			if (xData.server_type() == NF_SERVER_TYPES::NF_ST_WORLD
				&& nCurArea == nAreaID)
			{
				NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xData.server_id());
				if (!pServerData)
				{
					pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
					mWorldMap.AddElement(xData.server_id(), pServerData);
				}

				pServerData->nFD = nSockIndex;
				*(pServerData->pData) = xData;

				m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
			}
		}
	}

	//sync to proxy
	SynWorldToProxy();

	//sync to game
	SynWorldToGame();

}

bool NFWorldNet_ServerModule::Shut()
{

    return true;
}

bool NFWorldNet_ServerModule::Execute()
{

	if (mnLastCheckTime + 10 > GetPluginManager()->GetNowTime())
	{
		return true;
	}

	SynDBToGame();
	SynGameToProxy();
	SynWorldToProxy();
	SynWorldToGame();
	SynWorldToDB();


    LogGameServer();

	return true;
}

void NFWorldNet_ServerModule::OnGameServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			return;
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		NFGUID nPlayerID;
		NFMsg::ServerInfoReportList xMsg;
		if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
		{
			return;
		}

		for (int i = 0; i < xMsg.server_list_size(); ++i)
		{
			const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
			const int nAreaID = m_pElementModule->GetPropertyInt(xData.server_name(), NFrame::Server::Area());
			if (nAreaID == nCurArea)
			{
				NF_SHARE_PTR<ServerData> pServerData = mGameMap.GetElement(xData.server_id());
				if (!pServerData)
				{

					pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
					mGameMap.AddElement(xData.server_id(), pServerData);
				}

				pServerData->nFD = nSockIndex;
				*(pServerData->pData) = xData;

				m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
			}
			else
			{
				m_pLogModule->LogError(NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
			}
		}
	}

    SynGameToProxy();
}

void NFWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        mGameMap.RemoveElement(xData.server_id());

        m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }
}

void NFWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(xData.server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

    SynGameToProxy();
}

void NFWorldNet_ServerModule::OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			return;
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		NFGUID nPlayerID;
		NFMsg::ServerInfoReportList xMsg;
		if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
		{
			return;
		}

		for (int i = 0; i < xMsg.server_list_size(); ++i)
		{
			const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
			const int nAreaID = m_pElementModule->GetPropertyInt(xData.server_name(), NFrame::Server::Area());
			if (nAreaID == nCurArea)
			{
				NF_SHARE_PTR<ServerData> pServerData = mProxyMap.GetElement(xData.server_id());
				if (!pServerData)
				{
					pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
					mProxyMap.AddElement(xData.server_id(), pServerData);
				}

				pServerData->nFD = nSockIndex;
				*(pServerData->pData) = xData;

				m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

				SynGameToProxy(nSockIndex);
			}
			else
			{
				m_pLogModule->LogError(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
			}
		}
	}
}

void NFWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        mGameMap.RemoveElement(xData.server_id());

        m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
    }
}

void NFWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(xData.server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mProxyMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

        SynGameToProxy(nSockIndex);
    }
}

void NFWorldNet_ServerModule::OnDBServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			return;
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		NFGUID nPlayerID;
		NFMsg::ServerInfoReportList xMsg;
		if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
		{
			return;
		}

		for (int i = 0; i < xMsg.server_list_size(); ++i)
		{
			const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
			const int nAreaID = m_pElementModule->GetPropertyInt(xData.server_name(), NFrame::Server::Area());
			if (nAreaID == nCurArea)
			{

				NF_SHARE_PTR<ServerData> pServerData = mDBMap.GetElement(xData.server_id());
				if (!pServerData)
				{
					pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
					mDBMap.AddElement(xData.server_id(), pServerData);
				}

				pServerData->nFD = nSockIndex;
				*(pServerData->pData) = xData;

				m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "DBServerRegistered");
			}
			else
			{
				m_pLogModule->LogError(NFGUID(0, xData.server_id()), xData.server_name(), "DBServerRegistered");
			}
		}

		SynDBToGame();
	}
}

void NFWorldNet_ServerModule::OnDBServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		mDBMap.RemoveElement(xData.server_id());

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
	}
}

void NFWorldNet_ServerModule::OnRefreshDBServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		NF_SHARE_PTR<ServerData> pServerData = mDBMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mDBMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

		SynDBToGame(nSockIndex);
	}
}

void NFWorldNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFWorldNet_ServerModule::SynGameToProxy()
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.First();
    while (pServerData)
    {
        SynGameToProxy(pServerData->nFD);

        pServerData = mProxyMap.Next();
    }
}

void NFWorldNet_ServerModule::SynGameToProxy(const NFSOCK nFD)
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData)
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mGameMap.Next();
    }

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, nFD);
}

void NFWorldNet_ServerModule::SynWorldToProxy()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		SynWorldToProxy(pServerData->nFD);

		pServerData = mProxyMap.Next();
	}
}

void NFWorldNet_ServerModule::SynWorldToProxy(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, nFD);
}

void NFWorldNet_ServerModule::SynWorldToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		if (pServerData->pData->server_state() != NFMsg::EServerState::EST_MAINTEN
			&& pServerData->pData->server_state() != NFMsg::EServerState::EST_CRASH)
		{
			SynWorldToGame(pServerData->nFD);
		}

		pServerData = mGameMap.Next();
	}
}

void NFWorldNet_ServerModule::SynWorldToGame(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, nFD);
}


void NFWorldNet_ServerModule::SynWorldToDB()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mDBMap.First();
	while (pServerData)
	{
		if (pServerData->pData->server_state() != NFMsg::EServerState::EST_MAINTEN
			&& pServerData->pData->server_state() != NFMsg::EServerState::EST_CRASH)
		{
			SynWorldToDB(pServerData->nFD);
		}

		pServerData = mDBMap.Next();
	}
}

void NFWorldNet_ServerModule::SynWorldToDB(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{

		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, nFD);
}

void NFWorldNet_ServerModule::SynDBToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		if (pServerData->pData->server_state() != NFMsg::EServerState::EST_MAINTEN
			&& pServerData->pData->server_state() != NFMsg::EServerState::EST_CRASH)
		{
			SynDBToGame(pServerData->nFD);
		}

		pServerData = mGameMap.Next();
	}
}

void NFWorldNet_ServerModule::SynDBToGame(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;
	
	NF_SHARE_PTR<ServerData> pServerData = mDBMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);
		
		pServerData = mDBMap.Next();
	}
	
	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, nFD);
}

void NFWorldNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

			ServerReport(pServerData->pData->server_id(), NFMsg::EST_CRASH);
            SynGameToProxy();
            break;
        }

        pServerData = mGameMap.Next();
    }

    //////////////////////////////////////////////////////////////////////////

    pServerData =  mProxyMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

            int nServerID = pServerData->pData->server_id();
            mProxyMap.RemoveElement(nServerID);

			ServerReport(pServerData->pData->server_id(), NFMsg::EST_CRASH);
			SynGameToProxy();
            break;
        }

        pServerData = mProxyMap.Next();
    }


	//////////////////////////////////////////////////////////////////////////

	pServerData = mDBMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->nFD)
		{
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

			int nServerID = pServerData->pData->server_id();
			mDBMap.RemoveElement(nServerID);

			ServerReport(pServerData->pData->server_id(), NFMsg::EST_CRASH);
			SynDBToGame();
			break;
		}

		pServerData = mDBMap.Next();
	}
}

void NFWorldNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFWorldNet_ServerModule::LogGameServer()
{
    mnLastCheckTime = GetPluginManager()->GetNowTime();

    m_pLogModule->LogInfo(NFGUID(), "--------------------Begin Log GameServer Info", "");

    NF_SHARE_PTR<ServerData> pGameData = mGameMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogInfo(NFGUID(), stream);

        pGameData = mGameMap.Next();
    }

    m_pLogModule->LogInfo(NFGUID(), "--------------------End Log GameServer Info", "");

    m_pLogModule->LogInfo(NFGUID(), "--------------------Begin Log ProxyServer Info", "");

    pGameData = mProxyMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogInfo(NFGUID(), stream);

        pGameData = mProxyMap.Next();
    }

    m_pLogModule->LogInfo(NFGUID(), "--------------------End Log ProxyServer Info", "");


	m_pLogModule->LogInfo(NFGUID(), "--------------------Begin Log DBServer Info", "");

	pGameData = mDBMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

		m_pLogModule->LogInfo(NFGUID(), stream);

		pGameData = mDBMap.Next();
	}

	m_pLogModule->LogInfo(NFGUID(), "--------------------End Log DBServer Info", "");
}


void NFWorldNet_ServerModule::OnOnlineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOnlineNotify);

    NFGUID selfId = NFINetModule::PBToNF(xMsg.self());

    NF_SHARE_PTR<PlayerData> playerData = mPlayersData.GetElement(selfId);
    if (playerData)
    {
		playerData->name = xMsg.name();
		playerData->bp = xMsg.bp();

    }
	else
	{
		playerData = NF_SHARE_PTR<PlayerData>(NF_NEW PlayerData(selfId));

		playerData->name = xMsg.name();
		playerData->bp = xMsg.bp();

		mPlayersData.AddElement(selfId, playerData);
	}

	playerData->OnLine(xMsg.game(), xMsg.proxy());
}

void NFWorldNet_ServerModule::OnOfflineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOfflineNotify);
    NFGUID self = NFINetModule::PBToNF(xMsg.self());


	NF_SHARE_PTR<PlayerData> playerData = mPlayersData.GetElement(self);
	if (playerData)
	{
		playerData->OffLine();
	}

}

void NFWorldNet_ServerModule::OnTransmitServerReport(const NFSOCK nFd, const int msgId, const char *buffer, const uint32_t nLen)
{
    NFGUID xGUID;
	NFMsg::ServerInfoReport msg;
	if (!m_pNetModule->ReceivePB(msgId, buffer, nLen, msg, xGUID))
	{
		return;
	}


	m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::STS_SERVER_REPORT, msg, NFGUID());

}

bool NFWorldNet_ServerModule::SendMsgToGame(const NFGUID nPlayer, const int msgID, const std::string& xData)
{
    NF_SHARE_PTR<PlayerData> playerData = mPlayersData.GetElement(nPlayer);
    if (playerData)
    {
        NF_SHARE_PTR<ServerData> pData = mGameMap.GetElement(playerData->gameID);
        if (pData)
        {
            const NFSOCK nFD = pData->nFD;
            m_pNetModule->SendMsg(msgID, xData, nFD, nPlayer);

            return true;
        }
    }

    return false;
}

bool NFWorldNet_ServerModule::SendMsgToGame(const NFGUID nPlayer, const int msgID, google::protobuf::Message& xData)
{
	NF_SHARE_PTR<PlayerData> playerData = mPlayersData.GetElement(nPlayer);
	if (playerData)
	{
		NF_SHARE_PTR<ServerData> pData = mGameMap.GetElement(playerData->gameID);
		if (pData)
		{
			const NFSOCK nFD = pData->nFD;
			m_pNetModule->SendMsgPB(msgID, xData, nFD, nPlayer);

			return true;
		}
	}

	return false;
}

bool NFWorldNet_ServerModule::SendMsgToGame(const NFDataList& argObjectVar, const int msgID, google::protobuf::Message& xData)
{
    for (int i = 0; i < argObjectVar.GetCount(); i++)
    {
        const NFGUID& nPlayer = argObjectVar.Object(i);
		SendMsgToGame(nPlayer, msgID, xData);
    }

    return true;
}

NF_SHARE_PTR<ServerData> NFWorldNet_ServerModule::GetSuitProxyForEnter()
{
	int nConnectNum = 99999;
	NF_SHARE_PTR<ServerData> pReturnServerData;

	NF_SHARE_PTR<ServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (pServerData->pData->server_cur_count() < nConnectNum)
		{
			nConnectNum = pServerData->pData->server_cur_count();
			pReturnServerData = pServerData;
		}

		pServerData = mProxyMap.Next();
	}

	return pReturnServerData;
}

int NFWorldNet_ServerModule::GetPlayerGameID(const NFGUID self)
{
	NF_SHARE_PTR<PlayerData> playerData = mPlayersData.GetElement(self);
	if (playerData)
	{
		return playerData->gameID;
	}

	return 0;
}


const std::vector<NFGUID>& NFWorldNet_ServerModule::GetOnlinePlayers()
{
	static std::vector<NFGUID> players;
	players.clear();

	NF_SHARE_PTR<PlayerData> playerData = mPlayersData.First();
	while (playerData)
	{
		players.push_back(playerData->self);

		playerData = mPlayersData.Next();
	}

    return players;
}

NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> NFWorldNet_ServerModule::GetPlayerData(const NFGUID& id)
{
	return mPlayersData.GetElement(id);
}

void NFWorldNet_ServerModule::ServerReport(int reportServerId, NFMsg::EServerState serverStatus)
{
	std::shared_ptr<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (reportServerId == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strId);
				reqMsg.set_server_cur_count(0);
				reqMsg.set_server_ip(strIP);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(nMaxConnect);
				reqMsg.set_server_state(serverStatus);
				reqMsg.set_server_type(nServerType);


				m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::STS_SERVER_REPORT, reqMsg, NFGUID());
			}
		}
	}
}
