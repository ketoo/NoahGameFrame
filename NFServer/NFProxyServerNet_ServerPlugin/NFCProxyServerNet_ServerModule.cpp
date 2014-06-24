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
    mnConnectContainer = -1;
    mnWantToConnectContainer = -2;
    mnGameContainerID = -3;

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

    m_pEventProcessModule->AddClassCallBack("WantToConnect", this, &NFCProxyServerNet_ServerModule::OnWantToConnectObjectEvent);
    //m_pEventProcessModule->AddClassCallBack("ClientConnect", this, &NFCProxyServerNet_ServerModule::OnConnectObjectEvent);

    //////////////////////////////////////////////////////////////////////////

    m_pKernelModule->CreateContainer(mnConnectContainer, "");
    m_pKernelModule->CreateContainer(mnWantToConnectContainer, "");

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyServerNet_ServerModule::OnRecivePack, &NFCProxyServerNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}

    return true;
}

bool NFCProxyServerNet_ServerModule::Shut()
{
    m_pNet->Final();

    return true;
}

bool NFCProxyServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

int NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    m_pKernelModule->DestroyObject(self);

    return 0;
}

int NFCProxyServerNet_ServerModule::GetServerPort()
{
    return 0;//GetPort();
}

int NFCProxyServerNet_ServerModule::OnWantToConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
    if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
    {
        //60秒客户端还不连接就删掉[还是一直等着他连接算了把]
        m_pKernelModule->AddHeartBeat( self, "HB_OnPlayerWantToConnect", this, &NFCProxyServerNet_ServerModule::HB_OnPlayerWantToConnect, NFCValueList(), 60.0f, 1 );
    }
    else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        //销毁
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnConnectKeyProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ReqAccountLogin xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    NFCValueList varObjectList;
    m_pKernelModule->GetObjectByProperty(mnWantToConnectContainer, "Account", NFCValueList() << xMsg.account(), varObjectList);
    if (varObjectList.GetCount() == 1)
    {
        NFIDENTID ident = varObjectList.ObjectVal(0);
        const std::string& strConnectKey = m_pKernelModule->QueryPropertyString(ident, "ConnectKey");
        if (strConnectKey == xMsg.security_code())
        {
            //可以进入,设置标志，选单服,心跳延迟,进入gs创建角色和删除角色,这里只是转发
            NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
            if (pNetObject)
            {
                pNetObject->SetLogicState(1);

                NFMsg::AckEventResult xMsg;
                xMsg.set_event_code(NFMsg::EGEC_VERIFY_KEY_SUCCESS);
                SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_KEY, xMsg, msg.GetFd());
            }
        }
        else
        {
            NFMsg::AckEventResult xMsg;
            xMsg.set_event_code(NFMsg::EGEC_VERIFY_KEY_FAIL);
            SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_KEY, xMsg, msg.GetFd());
        }
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
        
    default:
        OnTranspondProcess(msg);
        break;
    }
	return 0;

}

int NFCProxyServerNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }


	return 0;

}

void NFCProxyServerNet_ServerModule::OnClientDisconnect( const int nAddress )
{

}

int NFCProxyServerNet_ServerModule::HB_OnPlayerWantToConnect( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    m_pKernelModule->DestroyObject( self );

    return 0;
}

int NFCProxyServerNet_ServerModule::OnTranspondProcess( const NFIPacket& msg )
{
    NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
    if (pNetObject)
    {
        //得到gs_id
        int nUserData = pNetObject->GetUserData();
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
    int64_t nPlayerID = 0;
    NFMsg::ReqSelectServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    NFCValueList varGameServerList;
    m_pKernelModule->GetObjectByProperty(mnGameContainerID, "ServerID", NFCValueList() << xMsg.world_id(), varGameServerList);
    if (varGameServerList.GetCount() == 1)
    {
        NFIDENTID ident = varGameServerList.ObjectVal(0);
        NFIObject* pObject = m_pKernelModule->GetObject(ident);
        if (pObject)
        {
            NFMsg::EServerState eState = (NFMsg::EServerState)pObject->QueryPropertyInt("State");
            if (eState != NFMsg::EST_CRASH)
            {
                //选择成功
                NetObject* pNetObject = this->GetNet()->GetNetObject(msg.GetFd());
                if (pNetObject)
                {
                    pNetObject->SetUserData(xMsg.world_id());

                    NFMsg::AckEventResult xMsg;
                    xMsg.set_event_code(NFMsg::EGameEventCode::EGEC_VERIFY_KEY_SUCCESS);
                    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, msg.GetFd());
                }
            }
        }
        
    }

    NFMsg::AckEventResult xSendMsg;
    xSendMsg.set_event_code(NFMsg::EGameEventCode::EGEC_SELECTSERVER_FAIL);
    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_SELECT_SERVER, xMsg, msg.GetFd());
    
    return 0;
}

int NFCProxyServerNet_ServerModule::OnReqServerListProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;
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
    if (pNetObject && pNetObject->GetLogicState() > 0)
    {
        //验证过的帐号才能获取服务器列表
        NFMsg::AckServerList xData;
        xData.set_type(NFMsg::RSLT_GAMES_ERVER);

        NFCValueList varGameServerList;
        m_pKernelModule->GetContainerOnLineList(mnGameContainerID, varGameServerList);
        if (varGameServerList.GetCount() > 0)
        {
            for (int i = 0; i < varGameServerList.GetCount(); i++)
            {
                NFIDENTID ident = varGameServerList.ObjectVal(i);
                NFIObject* pObject = m_pKernelModule->GetObject(ident);
                if (pObject)
                {
                    NFMsg::ServerInfo* pServerInfo = xData.add_info();
                    int nServerID = pObject->QueryPropertyInt("ServerID");
                    int nState = pObject->QueryPropertyInt("State");
                    const std::string& strName = pObject->QueryPropertyString("Name");

                    pServerInfo->set_name(strName);
                    pServerInfo->set_status((NFMsg::EServerState)nState);
                    pServerInfo->set_server_id(nServerID);
                    pServerInfo->set_wait_count(0);
                }
            }
        }

        SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLS_LIST, xData, msg.GetFd());
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::Transpond(const NFIPacket& msg )
{
    NFMsg::MsgBase xMsg;
    xMsg.ParseFromString(msg.GetPacketData());
    for (int i = 0; i < xMsg.player_fd_list_size(); ++i)
    {
        GetNet()->SendMsg(msg, xMsg.player_fd_list(i));
    }

    if(xMsg.player_fd_list_size() <= 0)
    {
        GetNet()->SendMsg(msg, msg.GetFd());
    }

    return true;
}

void NFCProxyServerNet_ServerModule::OnClientConnected( const int nAddress )
{

}
