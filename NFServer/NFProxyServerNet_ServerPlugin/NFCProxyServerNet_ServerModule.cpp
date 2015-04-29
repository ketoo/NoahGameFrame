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
    
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pProxyToWorldModule = dynamic_cast<NFIProxyServerToWorldModule*>(pPluginManager->FindModule("NFCProxyServerToWorldModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));
	
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pProxyToWorldModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pUUIDModule);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("ProxyServer");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		if (xNameList.Get(0, strConfigName))
		{
			const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
			const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
			const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
			const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
			const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
			const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

			m_pUUIDModule->SetIdentID(nServerID);

			Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyServerNet_ServerModule::OnRecivePack, &NFCProxyServerNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);

		}
	}

    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_ENTERGAME_SUCESS, this, &NFCProxyServerNet_ServerModule::OnEnterGameSuccessEvent);


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

    bool bRet = m_pProxyToWorldModule->VerifyConnectData(xMsg.account(), xMsg.security_code());
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
            *xSendMsg.mutable_event_client() = NFToPB(pNetObject->GetClientID());//让前端记得自己的fd，后面有一些验证

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

int NFCProxyServerNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
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
        int nGameID = pNetObject->GetGameID();
        if (nGameID > 0)
        {
            if (!pNetObject->GetUserID().IsNull())
            {
                //掉线
                NFMsg::ReqLeaveGameServer xData;

                NFCPacket msg(this->GetNet()->GetHeadLen());
                NFMsg::MsgBase xMsg;
                //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
                *xMsg.mutable_player_id() = NFToPB(pNetObject->GetUserID());

                if (!xData.SerializeToString(xMsg.mutable_msg_data()))
                {
                    return;
                }

                std::string strMsg;
                if(!xMsg.SerializeToString(&strMsg))
                {
                    return;
                }

                if(!msg.EnCode(NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME, strMsg.c_str(), strMsg.length()))
                {
                    return;
                }

                m_pProxyToWorldModule->Transpond(nGameID, msg);
            }
        }

		NFIDENTID xClientIdent = pNetObject->GetClientID();
		mxClientIdent.RemoveElement(xClientIdent);
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
            return m_pProxyToWorldModule->Transpond(nUserData, msg);
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

    NFIProxyServerToWorldModule::GameData* pServerData = m_pProxyToWorldModule->GetGameData(xMsg.world_id());
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

        NFIProxyServerToWorldModule::GameDataMap& xGameDataMap = m_pProxyToWorldModule->GetGameDataMap();
        NFIProxyServerToWorldModule::GameData* pGameData = xGameDataMap.First();
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

    for (int i = 0; i < xMsg.player_client_list_size(); ++i)
    {
        NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(PBToNF(xMsg.player_client_list(i)));
        if (pFD)
        {
            GetNet()->SendMsg(msg, *pFD);
        }
    }

    if(xMsg.player_client_list_size() <= 0)
    {
        //playerid==ClientID;

        NF_SHARE_PTR<int> pFD = mxClientIdent.GetElement(PBToNF(xMsg.player_id()));
        if (pFD)
        {
            GetNet()->SendMsg(msg, *pFD);
        }
    }

    return true;
}

void NFCProxyServerNet_ServerModule::OnClientConnected( const int nAddress )
{
	NFIDENTID xClientIdent = m_pUUIDModule->CreateGUID();
    NetObject* pNetObject = GetNet()->GetNetObject(nAddress);
    if (pNetObject)
    {
        pNetObject->SetClientID(xClientIdent);
    }

	mxClientIdent.AddElement(xClientIdent, NF_SHARE_PTR<int>(new int(nAddress)));
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

    NFIProxyServerToWorldModule::GameData* pServerData = m_pProxyToWorldModule->GetGameData(xData.game_id());
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
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

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

    NFIProxyServerToWorldModule::GameData* pServerData = m_pProxyToWorldModule->GetGameData(xData.game_id());
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
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

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

    NFIProxyServerToWorldModule::GameData* pServerData = m_pProxyToWorldModule->GetGameData(xData.game_id());
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

    NFIProxyServerToWorldModule::GameData* pServerData = m_pProxyToWorldModule->GetGameData(xData.game_id());
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
            xMsg.mutable_player_id()->CopyFrom(NFToPB(pNetObject->GetClientID()));

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

int NFCProxyServerNet_ServerModule::OnEnterGameSuccessEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
{
    if (var.GetCount() != 2 || !var.TypeEx(TDATA_OBJECT, TDATA_OBJECT, TDATA_UNKNOWN))
    {
        return 1;
    }

    const NFIDENTID& xClientID = var.Object(0);
    const NFIDENTID& xPlayerID = var.Object(1);

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
