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
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REGISTERED, this, &NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_REGISTERED, this, &NFCWorldNet_ServerModule::OnGameServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_AI_REGISTERED, this, &NFCWorldNet_ServerModule::OnAIServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_AI_UNREGISTERED, this, &NFCWorldNet_ServerModule::OnAIServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_AI_REFRESH, this, &NFCWorldNet_ServerModule::OnRefreshAIServerInfoProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ONLINE_NOTIFY, this, &NFCWorldNet_ServerModule::OnOnlineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_OFFLINE_NOTIFY, this, &NFCWorldNet_ServerModule::OnOfflineProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_SERVER_REPORT, this, &NFCWorldNet_ServerModule::OnTranspondServerReport);

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
                const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
                const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

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

bool NFCWorldNet_ServerModule::Shut()
{

    return true;
}

bool NFCWorldNet_ServerModule::Execute()
{
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

void NFCWorldNet_ServerModule::OnAIServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		NF_SHARE_PTR<ServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "AI Registered");

		SynGameToProxy(nSockIndex);
	}
}

void NFCWorldNet_ServerModule::OnAIServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "AI UnRegistered");
	}
}

void NFCWorldNet_ServerModule::OnRefreshAIServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		NF_SHARE_PTR<ServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "AI Registered");

		SynGameToProxy(nSockIndex);
	}
}

int NFCWorldNet_ServerModule::OnLeaveGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

    return 0;
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
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }
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
            return;
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
            break;
        }

        pServerData = mProxyMap.Next();
    }
}

void NFCWorldNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFCWorldNet_ServerModule::LogGameServer()
{
    if (mnLastCheckTime + 10 > GetPluginManager()->GetNowTime())
    {
        return;
    }

    mnLastCheckTime = GetPluginManager()->GetNowTime();

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log GameServer Info", "");

    NF_SHARE_PTR<ServerData> pGameData = mGameMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

        pGameData = mGameMap.Next();
    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log GameServer Info", "");

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log ProxyServer Info", "");

    pGameData = mProxyMap.First();
    while (pGameData)
    {
        std::ostringstream stream;
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

        pGameData = mProxyMap.Next();
    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log ProxyServer Info", "");
}


void NFCWorldNet_ServerModule::OnOnlineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOnlineNotify);

}

void NFCWorldNet_ServerModule::OnOfflineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::RoleOfflineNotify);
}

void NFCWorldNet_ServerModule::OnTranspondServerReport(const NFSOCK nFd, const int msgId, const char* buffer, const uint32_t nLen)
{
    NFGUID xGUID;
	NFMsg::ServerInfoReport msg;
	if (!m_pNetModule->ReceivePB(msgId, buffer, nLen, msg, xGUID))
	{
		return;
	}

	std::shared_ptr<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(NF_SERVER_TYPES::NF_ST_MASTER);
	if (pServerData)
	{
		m_pNetClientModule->SendToServerByPB(pServerData->nGameID, NFMsg::EGMI_STS_SERVER_REPORT, msg);
	}
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

int NFCWorldNet_ServerModule::OnObjectListEnter(const NFDataList& self, const NFDataList& argVar)
{
    if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
    for (int i = 0; i < argVar.GetCount(); i++)
    {
        NFGUID identOld = argVar.Object(i);
        
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
        *(pEntryInfo->mutable_object_guid()) = NFINetModule::NFToPB(identOld);

        pEntryInfo->set_x((float)m_pKernelModule->GetPropertyFloat(identOld, "X"));
        pEntryInfo->set_y((float)m_pKernelModule->GetPropertyFloat(identOld, "Y"));
        pEntryInfo->set_z((float)m_pKernelModule->GetPropertyFloat(identOld, "Z"));
        pEntryInfo->set_career_type(m_pKernelModule->GetPropertyInt32(identOld, "Job"));
        pEntryInfo->set_player_state(m_pKernelModule->GetPropertyInt32(identOld, "State"));
        pEntryInfo->set_config_id(m_pKernelModule->GetPropertyString(identOld, "ConfigID"));
        pEntryInfo->set_scene_id(m_pKernelModule->GetPropertyInt32(identOld, "SceneID"));
        pEntryInfo->set_class_id(m_pKernelModule->GetPropertyString(identOld, "ClassName"));

    }

    if (xPlayerEntryInfoList.object_list_size() <= 0)
    {
        return 0;
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFGUID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }

        
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
    }

    return 1;
}


int NFCWorldNet_ServerModule::OnObjectListLeave(const NFDataList& self, const NFDataList& argVar)
{
    if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
    for (int i = 0; i < argVar.GetCount(); i++)
    {
        NFGUID identOld = argVar.Object(i);
        
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
        *pIdent = NFINetModule::NFToPB(argVar.Object(i));
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFGUID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }
        
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
    }

    return 1;
}


int NFCWorldNet_ServerModule::OnRecordEnter(const NFDataList& argVar, const NFDataList& argGameID, const NFGUID& self)
{
    if (argVar.GetCount() <= 0 || self.IsNull())
    {
        return 0;
    }

    if (argVar.GetCount() != argGameID.GetCount())
    {
        return 1;
    }

    NFMsg::MultiObjectRecordList xPublicMsg;
    NFMsg::MultiObjectRecordList xPrivateMsg;

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (pObject)
    {
        NFMsg::ObjectRecordList* pPublicData = NULL;
        NFMsg::ObjectRecordList* pPrivateData = NULL;

        NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
        NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
        while (pRecord)
        {
            if (!pRecord->GetPublic() && !pRecord->GetPrivate())
            {
                pRecord = pRecordManager->Next();
                continue;
            }

            NFMsg::ObjectRecordBase* pPrivateRecordBase = NULL;
            NFMsg::ObjectRecordBase* pPublicRecordBase = NULL;
            if (pRecord->GetPublic())
            {
                if (!pPublicData)
                {
                    pPublicData = xPublicMsg.add_multi_player_record();
                    *(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(self);
                }
                pPublicRecordBase = pPublicData->add_record_list();
                pPublicRecordBase->set_record_name(pRecord->GetName());

                OnRecordEnterPack(pRecord, pPublicRecordBase);
            }

            if (pRecord->GetPrivate())
            {
                if (!pPrivateData)
                {
                    pPrivateData = xPrivateMsg.add_multi_player_record();
                    *(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(self);
                }
                pPrivateRecordBase = pPrivateData->add_record_list();
                pPrivateRecordBase->set_record_name(pRecord->GetName());

                OnRecordEnterPack(pRecord, pPrivateRecordBase);
            }

            pRecord = pRecordManager->Next();
        }


        for (int i = 0; i < argVar.GetCount(); i++)
        {
            const NFGUID& identOther = argVar.Object(i);
            const int nGameID = (int)argGameID.Int(i);
            if (self == identOther)
            {
                if (xPrivateMsg.multi_player_record_size() > 0)
                {
                    SendMsgToGame(nGameID, NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPrivateMsg, identOther);
                }
            }
            else
            {
                if (xPublicMsg.multi_player_record_size() > 0)
                {
                    SendMsgToGame(nGameID, NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPublicMsg, identOther);
                }
            }
        }
    }

    return 0;
}


bool NFCWorldNet_ServerModule::OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase)
{
    if (!pRecord || !pObjectRecordBase)
    {
        return false;
    }

    for (int i = 0; i < pRecord->GetRows(); i ++)
    {
        if (pRecord->IsUsed(i))
        {
            
            NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
            pAddRowStruct->set_row(i);

            for (int j = 0; j < pRecord->GetCols(); j++)
            {
                
                NFDataList valueList;
                NFDATA_TYPE eType = pRecord->GetColType(j);
                switch (eType)
                {
                    case NFDATA_TYPE::TDATA_INT:
                    {
                        int64_t nValue = pRecord->GetInt(i, j);
                        //if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pAddData = pAddRowStruct->add_record_int_list();
                            pAddData->set_row(i);
                            pAddData->set_col(j);
                            pAddData->set_data(nValue);
                        }
                    }
                    break;
                    case NFDATA_TYPE::TDATA_FLOAT:
                    {
                        double dwValue = pRecord->GetFloat(i, j);
                        //if ( dwValue < -0.01f || dwValue > 0.01f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
                            pAddData->set_row(i);
                            pAddData->set_col(j);
                            pAddData->set_data(dwValue);
                        }
                    }
                    break;
                    case NFDATA_TYPE::TDATA_STRING:
                    {
                        const std::string& strData = pRecord->GetString(i, j);
                        //if ( !strData.empty() )
                        {
                            NFMsg::RecordString* pAddData = pAddRowStruct->add_record_string_list();
                            pAddData->set_row(i);
                            pAddData->set_col(j);
                            pAddData->set_data(strData);
                        }
                    }
                    break;
                    case NFDATA_TYPE::TDATA_OBJECT:
                    {
                        NFGUID ident = pRecord->GetObject(i, j);
                        //if ( !ident.IsNull() )
                        {
                            NFMsg::RecordObject* pAddData = pAddRowStruct->add_record_object_list();
                            pAddData->set_row(i);
                            pAddData->set_col(j);
                            *(pAddData->mutable_data()) = NFINetModule::NFToPB(ident);
                        }
                    }
                    break;
                    default:
                        break;
                }
            }
        }
    }

    return true;
}

int NFCWorldNet_ServerModule::OnPropertyEnter(const NFDataList& argVar, const NFDataList& argGameID, const NFGUID& self)
{
    if (argVar.GetCount() <= 0 || self.IsNull())
    {
        return 0;
    }

    if (argVar.GetCount() != argGameID.GetCount())
    {
        return 1;
    }

    NFMsg::MultiObjectPropertyList xPublicMsg;
    NFMsg::MultiObjectPropertyList xPrivateMsg;

    
    
    
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (pObject)
    {
        NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
        NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

        *(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(self);
        *(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(self);

        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
        NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
        while (pPropertyInfo)
        {
            if (pPropertyInfo->Changed())
            {
                switch (pPropertyInfo->GetType())
                {
                    case TDATA_INT:
                    {
                        if (pPropertyInfo->GetPublic())
                        {
                            NFMsg::PropertyInt* pDataInt = pPublicData->add_property_int_list();
                            pDataInt->set_property_name(pPropertyInfo->GetKey());
                            pDataInt->set_data(pPropertyInfo->GetInt());
                        }

                        if (pPropertyInfo->GetPrivate())
                        {
                            NFMsg::PropertyInt* pDataInt = pPrivateData->add_property_int_list();
                            pDataInt->set_property_name(pPropertyInfo->GetKey());
                            pDataInt->set_data(pPropertyInfo->GetInt());
                        }
                    }
                    break;

                    case TDATA_FLOAT:
                    {
                        if (pPropertyInfo->GetPublic())
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
                            pDataFloat->set_property_name(pPropertyInfo->GetKey());
                            pDataFloat->set_data(pPropertyInfo->GetFloat());
                        }

                        if (pPropertyInfo->GetPrivate())
                        {
                            NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
                            pDataFloat->set_property_name(pPropertyInfo->GetKey());
                            pDataFloat->set_data(pPropertyInfo->GetFloat());
                        }
                    }
                    break;

                    case TDATA_STRING:
                    {
                        if (pPropertyInfo->GetPublic())
                        {
                            NFMsg::PropertyString* pDataString = pPublicData->add_property_string_list();
                            pDataString->set_property_name(pPropertyInfo->GetKey());
                            pDataString->set_data(pPropertyInfo->GetString());
                        }

                        if (pPropertyInfo->GetPrivate())
                        {
                            NFMsg::PropertyString* pDataString = pPrivateData->add_property_string_list();
                            pDataString->set_property_name(pPropertyInfo->GetKey());
                            pDataString->set_data(pPropertyInfo->GetString());
                        }
                    }
                    break;

                    case TDATA_OBJECT:
                    {
                        if (pPropertyInfo->GetPublic())
                        {
                            NFMsg::PropertyObject* pDataObject = pPublicData->add_property_object_list();
                            pDataObject->set_property_name(pPropertyInfo->GetKey());
                            *(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetObject());
                        }

                        if (pPropertyInfo->GetPrivate())
                        {
                            NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
                            pDataObject->set_property_name(pPropertyInfo->GetKey());
                            *(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetObject());
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            pPropertyInfo = pPropertyManager->Next();
        }

        for (int i = 0; i < argVar.GetCount(); i++)
        {
            const NFGUID& identOther = argVar.Object(i);
            const int nGameID = argGameID.Int32(i);
            if (self == identOther)
            {
                
                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPrivateMsg, identOther);
            }
            else
            {
                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPublicMsg, identOther);
            }

        }
    }

    return 0;
}

NF_SHARE_PTR<ServerData> NFCWorldNet_ServerModule::GetSuitProxyForEnter()
{
    return mProxyMap.First();
}

int NFCWorldNet_ServerModule::GetPlayerGameID(const NFGUID self)
{
    //to do
    return -1;
}

void NFCWorldNet_ServerModule::ServerReport(int reportServerId, NFMsg::EServerState serverStatus)
{
	std::shared_ptr<NFIClass> xLogicClass = m_pClassModule->GetElement("Server");
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, "Type");
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, "ServerID");
			if (reportServerId == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, "Port");
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, "MaxOnline");
				const std::string& strName = m_pElementModule->GetPropertyString(strId, "Name");
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, "IP");

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strName);
				reqMsg.set_server_cur_count(0);
				reqMsg.set_server_ip(strIP);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(nMaxConnect);
				reqMsg.set_server_state(serverStatus);
				reqMsg.set_server_type(nServerType);

				std::shared_ptr<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(NF_SERVER_TYPES::NF_ST_MASTER);
				if (pServerData)
				{
					m_pNetClientModule->SendToServerByPB(pServerData->nGameID, NFMsg::EGMI_STS_SERVER_REPORT, reqMsg);
				}
			}
		}
	}
}
