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
    mstrConfigIdent = "ProxyServer";

    return true;
}

bool NFCProxyServerNet_ServerModule::AfterInit()
{
    
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pProxyServerNet_ClientModule = dynamic_cast<NFIProxyServerNet_ClientModule*>(pPluginManager->FindModule("NFCProxyServerNet_ClientModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pProxyServerNet_ClientModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pElementInfoModule);

	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

	Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyServerNet_ServerModule::OnRecivePack, &NFCProxyServerNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);

    return true;
}

bool NFCProxyServerNet_ServerModule::Shut()
{

    return true;
}

bool NFCProxyServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return NFINetModule::Execute(fLasFrametime, fStartedTime);
}

int NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var )
{
    m_pKernelModule->DestroyObject(self);

    return 0;
}

int NFCProxyServerNet_ServerModule::OnConnectKeyProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ReqAccountLogin xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    bool bRet = m_pProxyServerNet_ClientModule->VerifyConnectData(xMsg.account(), xMsg.security_code());
    if (bRet)
    {
        //可以进入,设置标志，选单服,心跳延迟,进入gs创建角色和删除角色,这里只是转发
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
        if (pNetObject)
        {
            pNetObject->SetConnectKeyState(1);
            pNetObject->SetAccount(xMsg.account());

            NFMsg::AckEventResult xSendMsg;
            xSendMsg.set_event_code(NFMsg::EGEC_VERIFY_KEY_SUCCESS);
            //*xSendMsg.mutable_event_object() = msg.GetFd();//让前端记得自己的fd，后面有一些验证
            xSendMsg.set_event_arg(msg.GetFd());//让前端记得自己的fd，后面有一些验证

            SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_KEY, xSendMsg, msg.GetFd());
        }
    }
    else
    {
        GetNet()->CloseNetObject(msg.GetFd());
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    //看他连接在哪个gs，然后转发
    switch (msg.GetMsgHead()->GetMsgID())
    {
    case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_KEY:
        OnConnectKeyProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST:
        OnReqServerListProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_SELECT_SERVER:
        OnSelectServerProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST:
        OnReqRoleListProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE:
        OnReqCreateRoleProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_DELETE_ROLE:
        OnReqDelRoleProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME:
        OnReqEnterGameServer(msg);
        break;
    default:
        {
			NFMsg::MsgBase xMsg;
			if(!xMsg.ParseFromArray(msg.GetData(), msg.GetDataLen()))
			{
				char szData[MAX_PATH] = { 0 };
				sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", msg.GetMsgHead()->GetMsgID());
				LogRecive(szData);

				return 0;
			}
//             if (!VerifyProtocol(msg, msg.GetFd()))
//             {
//                 break;
//             }

            NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
            if (!pNetObject || pNetObject->GetConnectKeyState() <= 0 || pNetObject->GetGameID() <= 0)
            {
                //没验证key，没选择服务器
                break;
            }

            OnTranspondProcess(msg);
        }
        break;
    }

	return 0;
}

int NFCProxyServerNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }

	return 0;
}

void NFCProxyServerNet_ServerModule::OnClientDisconnect( const int nAddress )
{
    NetObject* pNetObject = this->GetNet()->GetNetObject(nAddress);
    if (pNetObject)
    {
        int nUserData = pNetObject->GetGameID();
        if (nUserData > 0)
        {
            //掉线
            NFMsg::ReqLeaveGameServer xData;

            NFCPacket msg(this->GetNet()->GetHeadLen());
            NFMsg::MsgBase xMsg;
            if (!xData.SerializeToString(xMsg.mutable_msg_data()))
            {
                return;
            }

            //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
            *xMsg.mutable_player_id() = NFToPB(pNetObject->GetUserID());

            std::string strMsg;
            if(!xMsg.SerializeToString(&strMsg))
            {
                return;
            }

            if(!msg.EnCode(NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME, strMsg.c_str(), strMsg.length()))
            {
                return;
            }

            m_pProxyServerNet_ClientModule->Transpond(nUserData, msg);
        }
    }
}

int NFCProxyServerNet_ServerModule::OnTranspondProcess( const NFIPacket& msg )
{
    NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
    if (pNetObject)
    {
        //得到gs_id
        int nUserData = pNetObject->GetGameID();
        if (nUserData > 0)
        {
            //广播给谁呢，看他在哪个服务器上
            return m_pProxyServerNet_ClientModule->Transpond(nUserData, msg);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnSelectServerProcess( const NFIPacket& msg )
{
    NFIDENTID nPlayerID;
    NFMsg::ReqSelectServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    NFIProxyServerNet_ClientModule::GameData* pServerData = m_pProxyServerNet_ClientModule->GetGameData(xMsg.world_id());
    if (NULL != pServerData && pServerData->eState != NFMsg::EST_CRASH)
    {
        //选择成功
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
        if (pNetObject)
        {
            pNetObject->SetGameID(xMsg.world_id());

            NFMsg::AckEventResult xMsg;
            xMsg.set_event_code(NFMsg::EGameEventCode::EGEC_SELECTSERVER_SUCCESS);
            SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, msg.GetFd());
            return 0;
        }
    }

    NFMsg::AckEventResult xSendMsg;
    xSendMsg.set_event_code(NFMsg::EGameEventCode::EGEC_SELECTSERVER_SUCCESS);
    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, msg.GetFd());
    
    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqServerListProcess( const NFIPacket& msg )
{
    NFIDENTID nPlayerID;
    NFMsg::ReqServerList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    if (xMsg.type() != NFMsg::RSLT_GAMES_ERVER)
    {
        return 0;
    }

    NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
    if (pNetObject && pNetObject->GetConnectKeyState() > 0)
    {
        //验证过的帐号才能获取服务器列表
        NFMsg::AckServerList xData;
        xData.set_type(NFMsg::RSLT_GAMES_ERVER);

        NFIProxyServerNet_ClientModule::GameDataMap& xGameDataMap = m_pProxyServerNet_ClientModule->GetGameDataMap();
        NFIProxyServerNet_ClientModule::GameData* pGameData = xGameDataMap.First();
        while (NULL != pGameData)
        {
            NFMsg::ServerInfo* pServerInfo = xData.add_info();

            pServerInfo->set_name(pGameData->strName);
            pServerInfo->set_status(pGameData->eState);
            pServerInfo->set_server_id(pGameData->nGameID);
            pServerInfo->set_wait_count(0);

            pGameData = xGameDataMap.Next();
        }

        SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLD_LIST, xData, msg.GetFd());
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::Transpond(const NFIPacket& msg )
{
    NFMsg::MsgBase xMsg;
    if(!xMsg.ParseFromArray(msg.GetData(), msg.GetDataLen()))
    {
        char szData[MAX_PATH] = { 0 };
        sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", msg.GetMsgHead()->GetMsgID());
        LogRecive(szData);

        return false;
    }

    for (int i = 0; i < xMsg.player_fd_list_size(); ++i)
    {
        GetNet()->SendMsg(msg, xMsg.player_fd_list(i));
    }

    if(xMsg.player_fd_list_size() <= 0)
    {
        //playerid==fd
        GetNet()->SendMsg(msg, xMsg.player_id().index());
    }

    return true;
}

void NFCProxyServerNet_ServerModule::OnClientConnected( const int nAddress )
{

}

int NFCProxyServerNet_ServerModule::OnReqRoleListProcess( const NFIPacket& msg )
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFIDENTID nPlayerID;
    NFMsg::ReqRoleList xData;
    if (!RecivePB(msg, xData, nPlayerID))
    {
        return 0;
    }

    NFIProxyServerNet_ClientModule::GameData* pServerData = m_pProxyServerNet_ClientModule->GetGameData(xData.game_id());
    if (NULL != pServerData && pServerData->eState != NFMsg::EST_CRASH)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
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
            NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
            pPlayerID->set_svrid(0);
            pPlayerID->set_index(msg.GetFd());

            std::string strMsg;
            if(!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            NFCPacket xPacket(GetNet()->GetHeadLen());
            if(!xPacket.EnCode(NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST, strMsg.c_str(), strMsg.length()))
            {
                return false;
            }

            xPacket.SetFd(msg.GetFd());

            OnTranspondProcess(xPacket);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqCreateRoleProcess( const NFIPacket& msg )
{
    //在没有正式进入游戏之前，nPlayerID都是FD


    NFIDENTID nPlayerID;
    NFMsg::ReqCreateRole xData;
    if (!RecivePB(msg, xData, nPlayerID))
    {
        return 0;
    }

    NFIProxyServerNet_ClientModule::GameData* pServerData = m_pProxyServerNet_ClientModule->GetGameData(xData.game_id());
    if (NULL != pServerData && pServerData->eState != NFMsg::EST_CRASH)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
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
            NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
            pPlayerID->set_svrid(0);
            pPlayerID->set_index(msg.GetFd());

            std::string strMsg;
            if(!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            NFCPacket xPacket(GetNet()->GetHeadLen());
            if(!xPacket.EnCode(NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE, strMsg.c_str(), strMsg.length()))
            {
                return false;
            }

            xPacket.SetFd(msg.GetFd());


            OnTranspondProcess(xPacket);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqDelRoleProcess( const NFIPacket& msg )
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFIDENTID nPlayerID;
    NFMsg::ReqDeleteRole xData;
    if (!RecivePB(msg, xData, nPlayerID))
    {
        return 0;
    }

    NFIProxyServerNet_ClientModule::GameData* pServerData = m_pProxyServerNet_ClientModule->GetGameData(xData.game_id());
    if (NULL != pServerData && pServerData->eState != NFMsg::EST_CRASH)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
        if (pNetObject
            && pNetObject->GetConnectKeyState() > 0
            && pNetObject->GetGameID() == xData.game_id()
            && pNetObject->GetAccount() == xData.account())
        {
            OnTranspondProcess(msg);
        }
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqEnterGameServer( const NFIPacket& msg )
{
    //在没有正式进入游戏之前，nPlayerID都是FD
    NFIDENTID nPlayerID;
    NFMsg::ReqEnterGameServer xData;
    if (!RecivePB(msg, xData, nPlayerID))
    {
        return 0;
    }

    NFIProxyServerNet_ClientModule::GameData* pServerData = m_pProxyServerNet_ClientModule->GetGameData(xData.game_id());
    if (NULL != pServerData && pServerData->eState != NFMsg::EST_CRASH)
    {
        //数据匹配
        NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
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
            NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
            pPlayerID->set_svrid(0);
            pPlayerID->set_index(msg.GetFd());

            std::string strMsg;
            if(!xMsg.SerializeToString(&strMsg))
            {
                return false;
            }

            NFCPacket xPacket(GetNet()->GetHeadLen());
            if(!xPacket.EnCode(NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, strMsg.c_str(), strMsg.length()))
            {
                return false;
            }

            xPacket.SetFd(msg.GetFd());

            OnTranspondProcess(xPacket);
        }
    }

    return 0;
}
