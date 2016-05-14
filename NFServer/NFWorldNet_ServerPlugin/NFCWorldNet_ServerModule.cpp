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

bool NFCWorldNet_ServerModule::Init()
{
    return true;
}

bool NFCWorldNet_ServerModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pWorldLogicModule = pPluginManager->FindModule<NFIWorldLogicModule>("NFCWorldLogicModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pClusterSQLModule = pPluginManager->FindModule<NFIClusterModule>("NFCMysqlClusterModule");
    m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>("NFCWorldNet_ServerModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pWorldLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pClusterSQLModule);
    assert(NULL != m_pWorldNet_ServerModule);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && pPluginManager->AppID() == nServerID)
            {
                const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

                int nRet = Initialization(this, &NFCWorldNet_ServerModule::OnRecivePack, &NFCWorldNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);
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

    return NFINetModule::Execute();
}

int NFCWorldNet_ServerModule::OnGameServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(xData.server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        mGameMap.RemoveElement(xData.server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(xData.server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
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

    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        mGameMap.RemoveElement(xData.server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(xData.server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");

        SynGameToProxy(nSockIndex);
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnLeaveGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

    return 0;
}

void NFCWorldNet_ServerModule::OnRecivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    switch (nMsgID)
    {
        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REFRESH:
            OnRefreshProxyServerInfoProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED:
            OnProxyServerRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED:
            OnProxyServerUnRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_REGISTERED:
            OnGameServerRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_UNREGISTERED:
            OnGameServerUnRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_REFRESH:
            OnRefreshGameServerInfoProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_ACK_ONLINE_NOTIFY:
            OnOnlineProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_ACK_OFFLINE_NOTIFY:
            OnOfflineProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        default:
            break;
    }
}

void NFCWorldNet_ServerModule::OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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
    while (pServerData.get())
    {
        SynGameToProxy(pServerData->nFD);

        pServerData = mProxyMap.Next();
    }
}

void NFCWorldNet_ServerModule::SynGameToProxy(const int nFD)
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData.get())
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mGameMap.Next();
    }

    SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::OnClientDisconnect(const int nAddress)
{
    //不管是game还是proxy都要找出来,替他反注册
    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData.get())
    {
        if (nAddress == pServerData->nFD)
        {
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

            SynGameToProxy();
            return;
        }

        pServerData = mGameMap.Next();
    }

    //////////////////////////////////////////////////////////////////////////

    int nServerID = 0;
    pServerData =  mProxyMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            nServerID = pServerData->pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    mProxyMap.RemoveElement(nServerID);
}

void NFCWorldNet_ServerModule::OnClientConnected(const int nAddress)
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


void NFCWorldNet_ServerModule::OnOnlineProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::RoleOnlineNotify);

}

void NFCWorldNet_ServerModule::OnOfflineProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::RoleOfflineNotify);
}

bool NFCWorldNet_ServerModule::SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer)
{
    NF_SHARE_PTR<ServerData> pData = mGameMap.GetElement(nGameID);
    if (pData.get())
    {
        const int nFD = pData->nFD;
        SendMsgPB(eMsgID, xData, nFD, nPlayer);
    }

    return true;
}

bool NFCWorldNet_ServerModule::SendMsgToGame(const NFIDataList& argObjectVar, const NFIDataList& argGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData)
{
    if (argGameID.GetCount() != argObjectVar.GetCount())
    {
        return false;
    }

    for (int i = 0; i < argObjectVar.GetCount(); i++)
    {
        const NFGUID& identOther = argObjectVar.Object(i);
        const NFINT64  nGameID = argGameID.Int(i);

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

int NFCWorldNet_ServerModule::OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar)
{
    if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
    for (int i = 0; i < argVar.GetCount(); i++)
    {
        NFGUID identOld = argVar.Object(i);
        //排除空对象
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
        *(pEntryInfo->mutable_object_guid()) = NFToPB(identOld);

        pEntryInfo->set_x(m_pKernelModule->GetPropertyFloat(identOld, "X"));
        pEntryInfo->set_y(m_pKernelModule->GetPropertyFloat(identOld, "Y"));
        pEntryInfo->set_z(m_pKernelModule->GetPropertyFloat(identOld, "Z"));
        pEntryInfo->set_career_type(m_pKernelModule->GetPropertyInt(identOld, "Job"));
        pEntryInfo->set_player_state(m_pKernelModule->GetPropertyInt(identOld, "State"));
        pEntryInfo->set_config_id(m_pKernelModule->GetPropertyString(identOld, "ConfigID"));
        pEntryInfo->set_scene_id(m_pKernelModule->GetPropertyInt(identOld, "SceneID"));
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

        //可能在不同的网关呢,得到后者所在的网关FD
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
    }

    return 1;
}


int NFCWorldNet_ServerModule::OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar)
{
    if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
    for (int i = 0; i < argVar.GetCount(); i++)
    {
        NFGUID identOld = argVar.Object(i);
        //排除空对象
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
        *pIdent = NFToPB(argVar.Object(i));
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFGUID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }
        //可能在不同的网关呢,得到后者所在的网关FD
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
    }

    return 1;
}


int NFCWorldNet_ServerModule::OnRecordEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self)
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
    if (pObject.get())
    {
        NFMsg::ObjectRecordList* pPublicData = NULL;
        NFMsg::ObjectRecordList* pPrivateData = NULL;

        NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
        NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
        while (pRecord.get())
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
                    *(pPublicData->mutable_player_id()) = NFToPB(self);
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
                    *(pPrivateData->mutable_player_id()) = NFToPB(self);
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
            const NFINT64 nGameID = argGameID.Int(i);
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
            //不管public还是private都要加上，不然public广播了那不是private就广播不了了
            NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
            pAddRowStruct->set_row(i);

            for (int j = 0; j < pRecord->GetCols(); j++)
            {
                //如果是0就不发送了，因为客户端默认是0
                NFCDataList valueList;
                TDATA_TYPE eType = pRecord->GetColType(j);
                switch (eType)
                {
                    case TDATA_TYPE::TDATA_INT:
                    {
                        int nValue = pRecord->GetInt(i, j);
                        //if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pAddData = pAddRowStruct->add_record_int_list();
                            pAddData->set_row(i);
                            pAddData->set_col(j);
                            pAddData->set_data(nValue);
                        }
                    }
                    break;
                    case TDATA_TYPE::TDATA_FLOAT:
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
                    case TDATA_TYPE::TDATA_STRING:
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
                    case TDATA_TYPE::TDATA_OBJECT:
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

int NFCWorldNet_ServerModule::OnPropertyEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self)
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

    //分为自己和外人
    //1.public发送给所有人
    //2.如果自己在列表中，再次发送private数据
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (pObject.get())
    {
        NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
        NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

        *(pPublicData->mutable_player_id()) = NFToPB(self);
        *(pPrivateData->mutable_player_id()) = NFToPB(self);

        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
        NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
        while (pPropertyInfo.get())
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
                            *(pDataObject->mutable_data()) = NFToPB(pPropertyInfo->GetObject());
                        }

                        if (pPropertyInfo->GetPrivate())
                        {
                            NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
                            pDataObject->set_property_name(pPropertyInfo->GetKey());
                            *(pDataObject->mutable_data()) = NFToPB(pPropertyInfo->GetObject());
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
            const NFINT64 nGameID = argGameID.Int(i);
            if (self == identOther)
            {
                //找到他所在网关的FD
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
