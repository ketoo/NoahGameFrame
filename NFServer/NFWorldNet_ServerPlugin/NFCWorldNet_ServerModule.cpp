// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCWorldNet_ServerModule.h"
#include "NFWorldNet_ServerPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCWorldNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

    return true;
}

bool NFCWorldNet_ServerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REGISTERED, this, &NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_REGISTERED, this, &NFCWorldNet_ServerModule::OnGameServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_DTW_DB_REGISTERED, this, &NFCWorldNet_ServerModule::OnDBServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_DTW_DB_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnDBServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_DTW_DB_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshDBServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ONLINE_NOTIFY, this, &NFCWorldNet_ServerModule::OnOnlineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_OFFLINE_NOTIFY, this, &NFCWorldNet_ServerModule::OnOfflineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_SERVER_REPORT, this, &NFCWorldNet_ServerModule::OnTransmitServerReport);

    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQSWICHSERVER, this, &NFCWorldNet_ServerModule::OnReqSwitchServer)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACKSWICHSERVER, this, &NFCWorldNet_ServerModule::OnAckSwitchServer)) { return false; }

    m_pNetModule->AddEventCallBack(this, &NFCWorldNet_ServerModule::OnSocketEvent);
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
                //const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
                //const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
                if (nRet < 0)
                {
                    std::ostringstream strLog;
                    strLog << "Cannot init server net, Port = " << nPort;
                    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
                    NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
                    exit(0);
                }
            }
        }
    }

    return true;
}

void NFCWorldNet_ServerModule::OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		if (xData.server_type() == NF_SERVER_TYPES::NF_ST_WORLD)
		{
			NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
				mWorldMap.AddElement(xData.server_id(), pServerData);
			}

			pServerData->nFD = nSockIndex;
			*(pServerData->pData) = xData;

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
		}
	}

	//sync to proxy

	SynWorldToProxy();

	//sync to game
	SynWorldToGame();

}

bool NFCWorldNet_ServerModule::Shut()
{

    return true;
}

bool NFCWorldNet_ServerModule::Execute()
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

void NFCWorldNet_ServerModule::OnGameServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

    SynGameToProxy();
}

void NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }
}

void NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

    SynGameToProxy();
}

void NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

        SynGameToProxy(nSockIndex);
    }
}

void NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
    }
}

void NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

        SynGameToProxy(nSockIndex);
    }
}

void NFCWorldNet_ServerModule::OnDBServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "DBServerRegistered");
	}

	SynDBToGame();
}

void NFCWorldNet_ServerModule::OnDBServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
	}
}

void NFCWorldNet_ServerModule::OnRefreshDBServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

		SynDBToGame(nSockIndex);
	}
}

void NFCWorldNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connected success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }
}

void NFCWorldNet_ServerModule::OnReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                                  const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
    nPlayerID = NFINetModule::PBToNF(xMsg.selfid());

    //const NFGUID nClientID = NFINetModule::PBToNF(xMsg.client_id());
    //const int nGateID = (int)xMsg.gate_serverid();
    //const int nSceneID = (int)xMsg.sceneid();
    SendMsgToGame((int)xMsg.target_serverid(), NFMsg::EGMI_REQSWICHSERVER, xMsg, nPlayerID);
}

void NFCWorldNet_ServerModule::OnAckSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                                  const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckSwitchServer);
    nPlayerID = NFINetModule::PBToNF(xMsg.selfid());

    SendMsgToGame((int)xMsg.self_serverid(), NFMsg::EGMI_ACKSWICHSERVER, xMsg, nPlayerID);
}

void NFCWorldNet_ServerModule::SynGameToProxy()
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.First();
    while (pServerData)
    {
        SynGameToProxy(pServerData->nFD);

        pServerData = mProxyMap.Next();
    }
}

void NFCWorldNet_ServerModule::SynGameToProxy(const NFSOCK nFD)
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData)
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mGameMap.Next();
    }

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::SynWorldToProxy()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		SynWorldToProxy(pServerData->nFD);

		pServerData = mProxyMap.Next();
	}
}

void NFCWorldNet_ServerModule::SynWorldToProxy(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::SynWorldToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		SynWorldToGame(pServerData->nFD);

		pServerData = mGameMap.Next();
	}
}

void NFCWorldNet_ServerModule::SynWorldToGame(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}


void NFCWorldNet_ServerModule::SynWorldToDB()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mDBMap.First();
	while (pServerData)
	{
		SynWorldToDB(pServerData->nFD);

		pServerData = mDBMap.Next();
	}
}

void NFCWorldNet_ServerModule::SynWorldToDB(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::SynDBToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		SynDBToGame(pServerData->nFD);

		pServerData = mGameMap.Next();
	}
}

void NFCWorldNet_ServerModule::SynDBToGame(const NFSOCK nFD)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mDBMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mDBMap.Next();
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
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
			int nServerID = pServerData->pData->server_id();
			mDBMap.RemoveElement(nServerID);

			ServerReport(pServerData->pData->server_id(), NFMsg::EST_CRASH);
			SynDBToGame();
			break;
		}

		pServerData = mDBMap.Next();
	}
}

void NFCWorldNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFCWorldNet_ServerModule::LogGameServer()
{


    mnLastCheckTime = GetPluginManager()->GetNowTime();

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------Begin Log GameServer Info", "");

    NF_SHARE_PTR<ServerData> pGameData = mGameMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

        pGameData = mGameMap.Next();
    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------End Log GameServer Info", "");

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------Begin Log ProxyServer Info", "");

    pGameData = mProxyMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

        pGameData = mProxyMap.Next();
    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------End Log ProxyServer Info", "");


	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------Begin Log DBServer Info", "");

	pGameData = mDBMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

		pGameData = mDBMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "--------------------End Log DBServer Info", "");
}


void NFCWorldNet_ServerModule::OnOnlineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOnlineNotify);

}

void NFCWorldNet_ServerModule::OnOfflineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOfflineNotify);
}

void NFCWorldNet_ServerModule::OnTransmitServerReport(const NFSOCK nFd, const int msgId, const char *buffer,
                                                      const uint32_t nLen)
{
    NFGUID xGUID;
	NFMsg::ServerInfoReport msg;
	if (!m_pNetModule->ReceivePB(msgId, buffer, nLen, msg, xGUID))
	{
		return;
	}


	m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::EGMI_STS_SERVER_REPORT, msg);

}

bool NFCWorldNet_ServerModule::SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer)
{
    NF_SHARE_PTR<ServerData> pData = mGameMap.GetElement(nGameID);
    if (pData)
    {
        const NFSOCK nFD = pData->nFD;
		m_pNetModule->SendMsgPB(eMsgID, xData, nFD, nPlayer);
    }

    return true;
}

bool NFCWorldNet_ServerModule::SendMsgToGame(const NFDataList& argObjectVar, const NFDataList& argGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData)
{
    if (argGameID.GetCount() != argObjectVar.GetCount())
    {
        return false;
    }

    for (int i = 0; i < argObjectVar.GetCount(); i++)
    {
        const NFGUID& identOther = argObjectVar.Object(i);
        const int nGameID = (int) argGameID.Int(i);

        SendMsgToGame(nGameID, eMsgID, xData, identOther);
    }

    return true;
}

bool NFCWorldNet_ServerModule::SendMsgToPlayer(const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer)
{
    int nGameID = GetPlayerGameID(nPlayer);
    if (nGameID < 0)
    {
        return false;
    }

    return SendMsgToGame(nGameID, eMsgID, xData, nPlayer);
}

NF_SHARE_PTR<ServerData> NFCWorldNet_ServerModule::GetSuitProxyForEnter()
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

int NFCWorldNet_ServerModule::GetPlayerGameID(const NFGUID self)
{
    //TODO
    return -1;
}

void NFCWorldNet_ServerModule::ServerReport(int reportServerId, NFMsg::EServerState serverStatus)
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
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strName);
				reqMsg.set_server_cur_count(0);
				reqMsg.set_server_ip(strIP);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(nMaxConnect);
				reqMsg.set_server_state(serverStatus);
				reqMsg.set_server_type(nServerType);


				m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_MASTER, NFMsg::EGMI_STS_SERVER_REPORT, reqMsg);
			}
		}
	}
}
