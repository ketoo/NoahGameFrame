// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCProxyServerNet_ServerModule.h"
#include "NFProxyServerNet_ServerPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

bool NFCProxyServerNet_ServerModule::Init()
{
    return true;
}

bool NFCProxyServerNet_ServerModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pProxyToWorldModule = pPluginManager->FindModule<NFIProxyServerToWorldModule>("NFCProxyServerToWorldModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pProxyServerToGameModule = pPluginManager->FindModule<NFIProxyServerToGameModule>("NFCProxyServerToGameModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pProxyToWorldModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pProxyServerToGameModule);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->AppID() == nServerID)
            {
                const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

                m_pUUIDModule->SetIdentID(nServerID);

                int nRet = Initialization(this, &NFCProxyServerNet_ServerModule::OnReciveClientPack, &NFCProxyServerNet_ServerModule::OnSocketClientEvent, nMaxConnect, nPort, nCpus);
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

bool NFCProxyServerNet_ServerModule::Shut()
{

    return true;
}

bool NFCProxyServerNet_ServerModule::Execute()
{
    return NFINetModule::Execute();
}

int NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var)
{
    m_pKernelModule->DestroyObject(self);

    return 0;
}

int NFCProxyServerNet_ServerModule::OnConnectKeyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ReqAccountLogin xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    bool bRet = m_pProxyToWorldModule->VerifyConnectData(xMsg.account(), xMsg.security_code());
    if (bRet)
    {
        //可以进入,设置标志，选单服,心跳延迟,进入gs创建角色和删除角色,这里只是转发
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject)
        {
            pNetObject->SetConnectKeyState(1);
            pNetObject->SetAccount(xMsg.account());

            NFMsg::AckEventResult xSendMsg;
            xSendMsg.set_event_code(NFMsg::EGEC_VERIFY_KEY_SUCCESS);
            *xSendMsg.mutable_event_client() = NFToPB(pNetObject->GetClientID());//让前端记得自己的fd，后面有一些验证

            SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_KEY, xSendMsg, nSockIndex);
        }
    }
    else
    {
        GetNet()->CloseNetObject(nSockIndex);
    }

    return 0;
}

void NFCProxyServerNet_ServerModule::OnReciveClientPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    m_pLogModule->LogNormal(NFILogModule::NLL_WARING_NORMAL, NFGUID(0, nSockIndex), "BeginMsg:", nMsgID, __FUNCTION__, __LINE__);
    //看他连接在哪个gs，然后转发
    switch (nMsgID)
    {
        case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_KEY:
            OnConnectKeyProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST:
            OnReqServerListProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_SELECT_SERVER:
            OnSelectServerProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST:
            OnReqRoleListProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE:
            OnReqCreateRoleProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_DELETE_ROLE:
            OnReqDelRoleProcess(nSockIndex, nMsgID, msg, nLen);
            break;
        case NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME:
            OnReqEnterGameServer(nSockIndex, nMsgID, msg, nLen);
            break;
        default:
        {
            NFMsg::MsgBase xMsg;
            if (!xMsg.ParseFromArray(msg, nLen))
            {
                char szData[MAX_PATH] = { 0 };
                sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
                LogRecive(szData);

                return;
            }

            NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
            if (!pNetObject || pNetObject->GetConnectKeyState() <= 0 || pNetObject->GetGameID() <= 0)
            {
                //state error
                break;
            }
            if (xMsg.has_hash_ident())
            {
                //special for distributed
                if (!pNetObject->GetHashIdentID().IsNull())
                {
                    NFCMachineNode xNode;
                    if (mxConsistentHash.GetSuitNode(pNetObject->GetHashIdentID().ToString(), xNode))
                    {
                        m_pProxyServerToGameModule->SendByServerID(xNode.GetDataID(), nMsgID, msg, nLen);
                    }
                }
                else
                {
                    NFGUID xHashIdent = NFINetModule::PBToNF(xMsg.hash_ident());

                    NFCMachineNode xNode;
                    if (mxConsistentHash.GetSuitNode(xHashIdent.ToString(), xNode))
                    {
                        m_pProxyServerToGameModule->SendByServerID(xNode.GetDataID(), nMsgID, msg, nLen);
                    }
                }
            }
            else
            {
                m_pProxyServerToGameModule->SendByServerID(pNetObject->GetGameID(), nMsgID, msg, nLen);
            }
        }
        break;
    }
    m_pLogModule->LogNormal(NFILogModule::NLL_WARING_NORMAL, NFGUID(0, nSockIndex), "EndMsg:", nMsgID, __FUNCTION__, __LINE__);
}

void NFCProxyServerNet_ServerModule::OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCProxyServerNet_ServerModule::OnClientDisconnect(const int nAddress)
{
    NetObject* pNetObject = this->GetNet()->GetNetObject(nAddress);
    if (pNetObject)
    {
        int nGameID = pNetObject->GetGameID();
        if (nGameID > 0)
        {
            if (!pNetObject->GetUserID().IsNull())
            {
                //掉线
                NFMsg::ReqLeaveGameServer xData;

                NFMsg::MsgBase xMsg;
                //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
                *xMsg.mutable_player_id() = NFToPB(pNetObject->GetUserID());

                if (!xData.SerializeToString(xMsg.mutable_msg_data()))
                {
                    return;
                }

                std::string strMsg;
                if (!xMsg.SerializeToString(&strMsg))
                {
                    return;
                }

                m_pProxyServerToGameModule->SendByServerID(nGameID, NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME, strMsg);
            }
        }

        NFGUID xClientIdent = pNetObject->GetClientID();
        mxClientIdent.RemoveElement(xClientIdent);
    }
}

int NFCProxyServerNet_ServerModule::OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ReqSelectServer xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    NF_SHARE_PTR<ConnectData> pServerData = m_pProxyServerToGameModule->GetServerNetInfo(xMsg.world_id());
    if (pServerData && ConnectDataState::NORMAL == pServerData->eState)
    {
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject)
        {
            //now this client bind a game server, after this time, all message will be sent to this game server who bind with client
            pNetObject->SetGameID(xMsg.world_id());

            NFMsg::AckEventResult xMsg;
            xMsg.set_event_code(NFMsg::EGameEventCode::EGEC_SELECTSERVER_SUCCESS);
            SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, nSockIndex);
            return 0;
        }
    }

    NFMsg::AckEventResult xSendMsg;
    xSendMsg.set_event_code(NFMsg::EGameEventCode::EGEC_SELECTSERVER_FAIL);
    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, nSockIndex);

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqServerListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ReqServerList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    if (xMsg.type() != NFMsg::RSLT_GAMES_ERVER)
    {
        return 0;
    }

    NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
    if (pNetObject && pNetObject->GetConnectKeyState() > 0)
    {
        //ack all gameserver data
        NFMsg::AckServerList xData;
        xData.set_type(NFMsg::RSLT_GAMES_ERVER);

        NFMapEx<int, ConnectData>& xServerList = m_pProxyServerToGameModule->GetServerList();
        ConnectData* pGameData = xServerList.FirstNude();
        while (NULL != pGameData)
        {
            if (ConnectDataState::NORMAL == pGameData->eState)
            {
                NFMsg::ServerInfo* pServerInfo = xData.add_info();

                pServerInfo->set_name(pGameData->strName);
                pServerInfo->set_status(NFMsg::EServerState::EST_NARMAL);
                pServerInfo->set_server_id(pGameData->nGameID);
                pServerInfo->set_wait_count(0);
            }

            pGameData = xServerList.NextNude();
        }

        SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLD_LIST, xData, nSockIndex);
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::Transpond(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFMsg::MsgBase xMsg;
    if (!xMsg.ParseFromArray(msg, nLen))
    {
        char szData[MAX_PATH] = { 0 };
        sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
        LogRecive(szData);

        return false;
    }

    //broadcast many palyer
    for (int i = 0; i < xMsg.player_client_list_size(); ++i)
    {
        NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(PBToNF(xMsg.player_client_list(i)));
        if (pFD)
        {
            if (xMsg.has_hash_ident())
            {
                NetObject* pNetObject = GetNet()->GetNetObject(*pFD);
                if (pNetObject)
                {
                    pNetObject->SetHashIdentID(NFINetModule::PBToNF(xMsg.hash_ident()));
                }
            }

            GetNet()->SendMsgWithOutHead(nMsgID, msg, nLen, *pFD);
        }
    }

    //send message to one player
    if (xMsg.player_client_list_size() <= 0)
    {
        //playerid==ClientID;

        NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(PBToNF(xMsg.player_id()));
        if (pFD)
        {
            if (xMsg.has_hash_ident())
            {
                NetObject* pNetObject = GetNet()->GetNetObject(*pFD);
                if (pNetObject)
                {
                    pNetObject->SetHashIdentID(NFINetModule::PBToNF(xMsg.hash_ident()));
                }
            }

            GetNet()->SendMsgWithOutHead(nMsgID, msg, nLen, *pFD);
        }
    }

    return true;
}

void NFCProxyServerNet_ServerModule::OnClientConnected(const int nAddress)
{
    NFGUID xClientIdent = m_pUUIDModule->CreateGUID();
    NetObject* pNetObject = GetNet()->GetNetObject(nAddress);
    if (pNetObject)
    {
        pNetObject->SetClientID(xClientIdent);
    }

    mxClientIdent.AddElement(xClientIdent, NF_SHARE_PTR<int>(new int(nAddress)));
}

int NFCProxyServerNet_ServerModule::OnReqRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFGUID nPlayerID;
    NFMsg::ReqRoleList xData;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nPlayerID))
    {
        return 0;
    }

    NF_SHARE_PTR<ConnectData> pServerData = m_pProxyServerToGameModule->GetServerNetInfo(xData.game_id());
    if (pServerData && ConnectDataState::NORMAL == pServerData->eState)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject
            && pNetObject->GetConnectKeyState() > 0
            && pNetObject->GetGameID() == xData.game_id()
            && pNetObject->GetAccount() == xData.account())
        {
            NFMsg::MsgBase xMsg;
            if (!xData.SerializeToString(xMsg.mutable_msg_data()))
            {
                return false;
            }

            //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

            std::string strMsg;
            if (!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            m_pProxyServerToGameModule->SendByServerID(pNetObject->GetGameID(), NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST, strMsg);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqCreateRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //在没有正式进入游戏之前，nPlayerID都是FD


    NFGUID nPlayerID;
    NFMsg::ReqCreateRole xData;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nPlayerID))
    {
        return 0;
    }

    NF_SHARE_PTR<ConnectData> pServerData = m_pProxyServerToGameModule->GetServerNetInfo(xData.game_id());
    if (pServerData && ConnectDataState::NORMAL == pServerData->eState)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject
            && pNetObject->GetConnectKeyState() > 0
            && pNetObject->GetGameID() == xData.game_id()
            && pNetObject->GetAccount() == xData.account())
        {
            NFMsg::MsgBase xMsg;
            if (!xData.SerializeToString(xMsg.mutable_msg_data()))
            {
                return false;
            }

            //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

            std::string strMsg;
            if (!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            m_pProxyServerToGameModule->SendByServerID(pNetObject->GetGameID(), nMsgID, strMsg);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqDelRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFGUID nPlayerID;
    NFMsg::ReqDeleteRole xData;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nPlayerID))
    {
        return 0;
    }

    NF_SHARE_PTR<ConnectData> pServerData = m_pProxyServerToGameModule->GetServerNetInfo(xData.game_id());
	if (pServerData && ConnectDataState::NORMAL == pServerData->eState)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject
            && pNetObject->GetConnectKeyState() > 0
            && pNetObject->GetGameID() == xData.game_id()
            && pNetObject->GetAccount() == xData.account())
        {
            m_pProxyServerToGameModule->SendByServerID(pNetObject->GetGameID(), nMsgID, std::string(msg, nLen));
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqEnterGameServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFGUID nPlayerID;
    NFMsg::ReqEnterGameServer xData;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nPlayerID))
    {
        return 0;
    }

    NF_SHARE_PTR<ConnectData> pServerData = m_pProxyServerToGameModule->GetServerNetInfo(xData.game_id());
    if (pServerData && ConnectDataState::NORMAL == pServerData->eState)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(nSockIndex);
        if (pNetObject
            && pNetObject->GetConnectKeyState() > 0
            && pNetObject->GetGameID() == xData.game_id()
            && pNetObject->GetAccount() == xData.account()
            && !xData.name().empty()
            && !xData.account().empty())
        {
            NFMsg::MsgBase xMsg;
            if (!xData.SerializeToString(xMsg.mutable_msg_data()))
            {
                return false;
            }

            //playerid在进入游戏之前都是FD，其他时候是真实的ID
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

            std::string strMsg;
            if (!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            m_pProxyServerToGameModule->SendByServerID(pNetObject->GetGameID(), NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, strMsg);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID)
{
    NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(xClientID);
    if (pFD)
    {
        NetObject* pNetObeject = GetNet()->GetNetObject(*pFD);
        if (pNetObeject)
        {
            pNetObeject->SetUserID(xPlayerID);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::SwitcheGameSuccess(const NFGUID xClientID, const NFGUID xPlayerID, const int nGameID)
{
	NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(xClientID);
	if (pFD)
	{
		NetObject* pNetObeject = GetNet()->GetNetObject(*pFD);
		if (pNetObeject)
		{
			pNetObeject->SetUserID(xPlayerID);
			pNetObeject->SetGameID(nGameID);
		}
	}

	return 0;
}
