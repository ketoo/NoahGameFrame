// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCWorldNet_ServerModule.h"
#include "NFWorldNet_ServerPlugin.h"

bool NFCWorldNet_ServerModule::Init()
{
    mnGameServerContainerID = - 3;
    mnDataBaseContainerID = - 4;
    mnProxyContainerID = -10;

    mstrConfigIdent = "WorldServer";

    return true;
}

bool NFCWorldNet_ServerModule::AfterInit()
{


    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pWorldLogicModule = dynamic_cast<NFIWorldLogicModule*>(pPluginManager->FindModule("NFCWorldLogicModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pWorldLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogicClassModule);

    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER, this, &NFCWorldNet_ServerModule::OnSelectServerEvent);

    //////////////////////////////////////////////////////////////////////////
    m_pKernelModule->CreateContainer(mnGameServerContainerID, "");
    m_pKernelModule->CreateContainer(mnDataBaseContainerID, "");
    m_pKernelModule->CreateContainer(mnProxyContainerID, "");

    // 	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, " -3: GameServerConnectContainer ");
    // 	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, " -4: DataBaseConnectContainer ");
    // 	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, " -10: ProxyConnectContainer  ");


    const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
    const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
    const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
    const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
    const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCWorldNet_ServerModule::OnRecivePack, &NFCWorldNet_ServerModule::OnSocketEvent);
    int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
    if (nRet <= 0)
    {
        assert(0);
    }
    return true;
}

bool NFCWorldNet_ServerModule::Shut()
{
    m_pNet->Final();

    return true;
}


// bool NFCWorldNet_ServerModule::DispatcherPacket(const NFIPacket& msg)
// {
//     unsigned int unMsgID = pMsgPacket->GetMessageId();
//     switch (unMsgID)
//     {
//         case GTW_GAME_REGISTERED:
//             OnGameServerRegisteredProcess(pMsgPacket);
//             break;
// 
//         case GTW_GAME_UNREGISTERED:
//             OnGameServerUnRegisteredProcess(pMsgPacket);
//             break;
// 
//         case GTW_GAME_REFRESH:
//             OnRefreshGameServerInfoProcess(pMsgPacket);
//             break;
// 
//             //         case MTD_LOAD_ROLEDATA:
//             //             OnLoadRoleDataBeginProcess(pMsgPacket);
//             //             break;
//             //
//             //         case DTM_LOAD_ROLEDATA_FINAL:
//             //             OnLoadRoleDataFinalProcess(pMsgPacket);
//             //             break;
//             //
//             //         case    MTD_SAVE_ROLEDATA:
//             //             OnSaveRoleDataProcess(pMsgPacket);
//             //             break;
//             //
//             //         case DTM_DATABASE_REGISTERED:
//             //             OnDataBaseRegisteredProcess(pMsgPacket);
//             //             break;
//             //
//             //         case DTM_DATABASE_UNREGISTERED:
//             //             OnDataBaseUnRegisteredProcess(pMsgPacket);
//             //             break;
// 
//         case    LTWG_PROXY_REGISTERED:
//             OnProxyServerRegisteredProcess(pMsgPacket);
//             break;
// 
//         case    LTWG_PROXY_UNREGISTERED:
//             OnProxyServerUnRegisteredProcess(pMsgPacket);
//             break;
// 
//         case    LTWG_PROXY_REFRESH:
//             OnRefreshProxyServerInfoProcess(pMsgPacket);
//             break;
//             //      case CTG_ENTER_WORLD:
//             //          OnWantToEnterWorldProcess( pMsgPacket );
//             //          break;
//         case  LTW_CLIENT_EXIT_WORLD:
//             OnExitServerProcess(pMsgPacket);
// 
//             //         case GTG_SWAP_GAMESERVER:
//             //             OnWantToSwapGSProcess(pMsgPacket);
//             //             break;
//         case CTG_LEAVE_GAME:
//             OnLeaveGameProcess(pMsgPacket);
//             break;
// 
//         default:
//             printf("NFNet || 非法消息:unMsgID=%d\n", pMsgPacket->GetMessageId());
//             break;
//     }
// 
//     return true;
// }

bool NFCWorldNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

int NFCWorldNet_ServerModule::GetGameServerObject(const int nServerID, NFIValueList& varObjectList)
{
    return m_pKernelModule->GetObjectByProperty(mnGameServerContainerID, "ServerID", NFCValueList() << nServerID, varObjectList);
}

int NFCWorldNet_ServerModule::OnGameServerRegisteredProcess(const NFIPacket& msg)
{
    int32_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetGameServerObject(pData->server_id(), varList);
        if (varList.GetCount() != 0)
        {
            NFIObject* pObject = m_pKernelModule->GetObject(varList.ObjectVal(i));
            if (pObject)
            {
                pObject->SetPropertyInt("State", NFMsg::EST_NARMAL);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
            }
            return 0;
        }

        NFIDENTID ident = varList.ObjectVal(0);
        if (ident.IsNull())
        {
            //默认都0号分组
            NFCValueList arg;
            arg << "FD" << msg.GetFd();
            NFIObject* pObject = m_pKernelModule->CreateObject(0, mnGameServerContainerID, 0, "GameServer", "", arg);
            if (pObject)
            {
                pObject->SetPropertyInt("ServerID", pData->server_id());
                pObject->SetPropertyString("Name", pData->server_name());
                pObject->SetPropertyInt("MaxOnline", pData->server_max_online());
                pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
                pObject->SetPropertyString("IP", pData->server_ip());
                pObject->SetPropertyInt("Port", pData->server_port());
                pObject->SetPropertyInt("State", NFMsg::EST_NARMAL);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
            }
        }
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const NFIPacket& msg)
{
    int32_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NFCValueList varList;
        GetGameServerObject(pData->server_id(), varList);
        for (int j = 0; j < varList.GetCount(); ++j)
        {
            m_pKernelModule->SetPropertyInt(varList.ObjectVal(j), "State", NFMsg::EST_NARMAL);

            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
        }
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const NFIPacket& msg)
{
    int32_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetGameServerObject(pData->server_id(), varList);
        if (varList.GetCount() <= 0)
        {
            return 0;
        }


        NFIDENTID ident = varList.ObjectVal(0);
        NFIObject* pObject = m_pKernelModule->GetObject(ident);
        if (pObject)
        {
            pObject->SetPropertyInt("State", pData->server_state());
            pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
            pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
        }
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (4 != var.GetCount()
        || !var.TypeEx(VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return 0;
    }

    const int nWorldID = var.IntVal(0);
    const int nSenderAddress = var.IntVal(1);
    const int nLoginID = var.IntVal(2);
    const std::string& strAccount = var.StringVal(3);

    //////////////////////////////////////////////////////////////////////////
    if (InThisWorld(strAccount))
    {
        return 0;
    }

    //指定一个网关,网管轮流来，那样分布均匀
    NFCValueList varObjectList;
    m_pKernelModule->GetContainerOnLineList(mnProxyContainerID, varObjectList);
    if (varObjectList.GetCount() > 0)
    {
        NFIDENTID ident = varObjectList.ObjectVal(0);
        NFList<std::string>* pList = GetElement(ident);
        if (pList)
        {
            pList->Add(strAccount);
            //每个玩家每次KEY都不一样
            std::string strConnectKey(strAccount);

            const int nFD = m_pKernelModule->QueryPropertyInt(ident, "FD");
            const std::string& strIP = m_pKernelModule->QueryPropertyString(ident, "ServerIP");
            //此port为proxyServer的服务器监听端口
            int nPort = m_pKernelModule->QueryPropertyInt(ident, "Port");

            NFMsg::AckConnectWorldResult xData;

            xData.set_world_id(nWorldID);
            xData.set_sender_ip(nSenderAddress);
            xData.set_login_id(nLoginID);
            xData.set_account(strAccount);

            xData.set_world_ip(strIP);
            xData.set_world_port(nPort);
            xData.set_world_key(strConnectKey);

            SendMsgPB(NFMsg::EGMI_REQ_CONNECT_WORLD, xData, nFD);

            //结果
            NFCValueList varResult;
            varResult << nWorldID << nSenderAddress << nLoginID << strAccount << strIP << nPort << strConnectKey;
            m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, varResult);
        }
    }
    else
    {
    }

    return 0;
}

int NFCWorldNet_ServerModule::GetProxyObject(const int nProxyID, NFIValueList& varObjectList)
{
    return m_pKernelModule->GetObjectByProperty(mnProxyContainerID, "ServerID", NFCValueList() << nProxyID, varObjectList);
}

int NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess(const NFIPacket& msg)
{
    int32_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NFCValueList varList;
        GetProxyObject(pData->server_id(), varList);
        if (varList.GetCount() != 0)
        {
            NFIObject* pObject = m_pKernelModule->GetObject(varList.ObjectVal(i));
            if (pObject)
            {
                pObject->SetPropertyInt("ServerID", pData->server_id());
                pObject->SetPropertyString("Name", pData->server_name());
                pObject->SetPropertyInt("MaxOnline", pData->server_max_online());
                pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
                pObject->SetPropertyString("IP", pData->server_ip());
                pObject->SetPropertyInt("Port", pData->server_port());
                pObject->SetPropertyInt("State", NFMsg::EST_NARMAL);

                //同步所有的gameserver给proxy
                int nFD = pObject->QueryPropertyInt("FD");
                SynGameToProxy(nFD);
                return 0;
            }
        }


        if (varList.GetCount() == 0)
        {

            //默认都0号分组
            NFCValueList arg;
            arg << "FD" << msg.GetFd();
            NFIObject* pObject = m_pKernelModule->CreateObject(0, mnProxyContainerID, 0, "ProxyServer", "", arg);
            if (pObject)
            {
                pObject->SetPropertyInt("ServerID", pData->server_id());
                pObject->SetPropertyString("Name", pData->server_name());
                pObject->SetPropertyInt("MaxOnline", pData->server_max_online());
                pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
                pObject->SetPropertyString("IP", pData->server_ip());
                pObject->SetPropertyInt("Port", pData->server_port());
                pObject->SetPropertyInt("State", NFMsg::EST_NARMAL);

                //同步所有的gameserver给proxy
                int nFD = pObject->QueryPropertyInt("FD");
                SynGameToProxy(nFD);
            }
        }
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFIPacket& msg)
{
    int32_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetProxyObject(pData->server_id(), varList);
        for (int j = 0; j < varList.GetCount(); ++j)
        {
            m_pKernelModule->DestroyObject(varList.ObjectVal(j));
        }
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFIPacket& msg)
{
    return 0;
}

int NFCWorldNet_ServerModule::OnLeaveGameProcess(const NFIPacket& msg)
{
    //     _tagPT_KEY_BASE_MSG baseMsg;
    //     pMsgPacket->DeCode(&baseMsg);
    //
    //     NFMsg:: xMsg;
    //     if ( !xMsg.ParseFromString( baseMsg.strSyncInfo ) )
    //     {
    //         return 0;
    //
    //     }
    //
    //     _tagCTG_LEAVE_GAME leaveGame;
    //     RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    //     leaveGame.DeCode(*pBitStream);
    //
    //     if (Find(leaveGame._szAccount))
    //     {
    //         Remove(leaveGame._szAccount);
    //     }
    return 0;
}

int NFCWorldNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    return 0;

}

int NFCWorldNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
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

void NFCWorldNet_ServerModule::SynGameToProxy()
{

    NFCValueList varGameObjectList;
    m_pKernelModule->GetContainerOnLineList(mnProxyContainerID, varGameObjectList);
    if (varGameObjectList.GetCount() > 0)
    {
        for (int i = 0; i < varGameObjectList.GetCount(); i++)
        {
            NFIDENTID ident = varGameObjectList.ObjectVal(i);
            int nFD = m_pKernelModule->QueryPropertyInt(ident, "FD");
            SynGameToProxy(nFD);
        }
    }
}

void NFCWorldNet_ServerModule::SynGameToProxy( const int nFD )
{
    NFCValueList varObjectList;
    m_pKernelModule->GetContainerOnLineList(mnGameServerContainerID, varObjectList);


    if (varObjectList.GetCount() > 0)
    {
        NFList<NFIPropertyManager*> list;

        for (int i = 0; i < varObjectList.GetCount(); i++)
        {
            NFIDENTID ident = varObjectList.ObjectVal(i);

            NFIObject* pObject = m_pKernelModule->GetObject(ident);
            if (pObject)
            {
                list.Add(pObject->GetPropertyManager());
            }
        }

        NFMsg::MultiObjectPropertyList xData;
        PropertyListToString(list, xData, E_CHECK_TYPE::ECT_PUBLIC, true);

        //////////////////////////////////////////////////////////////////////////
        SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);

    }
}

void NFCWorldNet_ServerModule::OnClientDisconnect( const int nAddress )
{
    NFCValueList varList;
    m_pKernelModule->GetObjectByProperty(mnGameServerContainerID, "FD", NFCValueList() << nAddress, varList);
    if (varList.GetCount() > 0)
    {
        m_pKernelModule->SetPropertyInt(varList.ObjectVal(0), "State", NFMsg::EST_CRASH);
    }

    varList.Clear();
    m_pKernelModule->GetObjectByProperty(mnProxyContainerID, "FD", NFCValueList() << nAddress, varList);
    if (varList.GetCount() > 0)
    {
        m_pKernelModule->DestroyObject(varList.ObjectVal(0));
    }
}

void NFCWorldNet_ServerModule::OnClientConnected( const int nAddress )
{


}

bool NFCWorldNet_ServerModule::InThisWorld( const std::string& strAccount )
{
    return false;
}

