// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerNet_ClientModule.h"
#include "NFGameServerNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFCGameServerNet_ClientModule::Init()
{
    mfLastHBTime = 0.0f;
    mnSocketFD = -1;
    return true;
}

bool NFCGameServerNet_ClientModule::Shut()
{

    return true;
}


bool NFCGameServerNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    KeepAlive(fLasFrametime);
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCGameServerNet_ClientModule::KeepAlive(float fLasFrametime)
{
    if (mfLastHBTime < 10.0f)
    {
        mfLastHBTime += fLasFrametime;
        return;
    }

    mfLastHBTime = 0.0f;

    NFMsg::ServerHeartBeat xMsg;
    xMsg.set_count(0);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, mnSocketFD);
}

void NFCGameServerNet_ClientModule::Register()
{
    //成功就注册

	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nServerID);
    pData->set_server_name(strName);
    pData->set_server_cur_count(0);
    pData->set_server_ip(strServerIP);
    pData->set_server_port(nPort);
    pData->set_server_max_online(50000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_GTW_GAME_REGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Register");
}

void NFCGameServerNet_ClientModule::UnRegister()
{
	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strSelfIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "IP");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nServerID);
    pData->set_server_name(strName);
    pData->set_server_cur_count(0);
    pData->set_server_ip(strSelfIP);
    pData->set_server_port(nPort);
    pData->set_server_max_online(50000);
    pData->set_server_state(NFMsg::EST_MAINTEN);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_GTW_GAME_UNREGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "UnRegister");

}

void NFCGameServerNet_ClientModule::RefreshWorldInfo()
{
//     _tagPT_KEY_BASE_MSG baseMsg;
//     NFMsg::ServerInfoReport xMsg;
// 
//     xMsg.set_server_id(m_pGameLogicModule->GetGameID());
//     xMsg.set_server_port(m_pGameServerModule->GetServerPort());
//     xMsg.set_server_cur_count(0);
//     xMsg.set_server_max_online(50000);
// 
//     if (xMsg.SerializeToString(&baseMsg.strSyncInfo))
//     {
//         baseMsg._unMsgID = GTW_GAME_REFRESH;
//         RakNet::BitStream oBitStream;
//         baseMsg.EnCode(&oBitStream);
//         SendBitStream(&oBitStream);
//     }
}

bool NFCGameServerNet_ClientModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

    //m_pEventProcessModule->AddEventCallBack(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, this, &NFCGameServerNet_ClientModule::OnDataLoadBeginEvent);
    //m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_WANTO_SWAP_GS, this, &NFCGameServerNet_ClientModule::OnSwapGSEvent);

    m_pKernelModule->ResgisterCommonClassEvent(this, &NFCGameServerNet_ClientModule::OnClassCommonEvent);
    
    // 连接world server
    mstrConfigIdent = "GameServer";

	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ClientModule::OnRecivePack, &NFCGameServerNet_ClientModule::OnSocketEvent);
    mnSocketFD = m_pNet->Initialization(strServerIP.c_str(), nServerPort);
    if (mnSocketFD < 0)
    {
        assert(0);
    }

    m_pKernelModule->CreateContainer(-2, "");

    return true;
}

int NFCGameServerNet_ClientModule::OnLoadRoleDataBeginProcess(const NFIPacket& msg)
{
    //     _tagPT_KEY_BASE_MSG baseMsg;
    //     pMsgPacket->DeCode(&baseMsg);
    //
    //     NFMsg:: xMsg;
    //     if ( !xMsg.ParseFromString( baseMsg.strSyncInfo ) )
    //     {
    //         return 0;
    //     }
    //
    //     _tagMTD_LOAD_ROLEDATA loadData;
    //     RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    //     loadData.DeCode(*pBitStream);
    //
    //     m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, NFCDataList() << loadData._szAccountName);

    return 0;
}

int NFCGameServerNet_ClientModule::OnLoadRoleDataFinalProcess(const NFIPacket& msg)
{

    //     _tagDTM_LOAD_ROLEDATA_FINAL loadFinal;
    //     RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    //     loadFinal.DeCode(*pBitStream);
    //
    //     //创建对象
    //     //开始认领内存，
    //     NFCDataList valueRoleList;
    //     valueRoleList << loadFinal._szAccountName;
    //     valueRoleList << loadFinal._szRoleName1;
    //     valueRoleList << loadFinal.nPlayerConfidIndex1;
    //     valueRoleList << loadFinal._szRoleName2;
    //     valueRoleList << loadFinal.nPlayerConfidIndex2;
    //     valueRoleList << loadFinal._szRoleName3;
    //     valueRoleList << loadFinal.nPlayerConfidIndex3;
    //     valueRoleList << loadFinal._szRoleName4;
    //     valueRoleList << loadFinal.nPlayerConfidIndex4;
    //
    //     m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_DATABASE_SERVER_LOADROE_FINAL_RESULTS, valueRoleList);


    //test
    //  _tagMTD_SAVE_ROLEDATA saveRole;
    //  RakNet::BitStream bitStream;
    //
    //  if ( strlen( loadFinal._szRoleName1 ) > 0 )
    //  {
    //      strcpy( saveRole._szRoleName, loadFinal._szRoleName1 );
    //  }
    //  else if ( strlen( loadFinal._szRoleName2 ) > 0 )
    //  {
    //      strcpy( saveRole._szRoleName, loadFinal._szRoleName2 );
    //  }
    //  else if ( strlen( loadFinal._szRoleName3 ) > 0 )
    //  {
    //      strcpy( saveRole._szRoleName, loadFinal._szRoleName3 );
    //  }
    //  else if ( strlen( loadFinal._szRoleName4 ) > 0 )
    //  {
    //      strcpy( saveRole._szRoleName, loadFinal._szRoleName4 );
    //  }
    //
    //  saveRole.EnCode( bitStream );
    //  SendBitStream( &bitStream );

    return 0;
}

//离开世界回到客户端时，产生
int NFCGameServerNet_ClientModule::OnDataLoadBeginEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    //1号gs发送
    //if (1 == m_pGameLogicModule->GetGameID())
    {
        //      _tagMTD_LOAD_ROLEDATA loadData;
        //
        //      strcpy(loadData._szAccountName, var.String(0));
        //      RakNet::BitStream oBitStream;
        //      loadData.EnCode(oBitStream);
        //
        //      SendBitStream(&oBitStream);

        //NOSQL直接加载
    }


    return 0;
}

// int NFCGameServerNet_ClientModule::OnToWorldEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
// {
//     if ( 4 != var.GetCount() )
//     {
//         return 0;
//     }
//
//  if (m_pGameLogicModule->GetGameID() == 1)
//  {
//      int nGameServer = var.Int( 0 );
//      const char* pstrAccount = var.String( 1 );
//      const char* pstrRoleName = var.String( 2 );
//      int nSceneIndex = var.Int( 3 );
//
//      RakNet::BitStream oBitStream;
//      _tagGTG_SWAP_GAMESERVER swapWorld;
//
//      swapWorld._nGameServerID = nGameServer;
//      strcpy( swapWorld._szAccount, pstrAccount );
//      strcpy( swapWorld._szRoleName, pstrRoleName );
//      swapWorld._nSceneIndex = nSceneIndex;
//
//      swapWorld.EnCode( oBitStream );
//      SendBitStream( &oBitStream );
//  }
//
//
//     return 0;
// }

int NFCGameServerNet_ClientModule::OnEnquireSceneInfoProcess(const NFIPacket& msg)
{
    //  _tagWTG_ENQUIRE_SCENE_INFO enquireSceneInfo;
    //  RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    //  enquireSceneInfo.DeCode( *pBitStream );


    return 0;
}

int NFCGameServerNet_ClientModule::OnSwapGSEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    //如果是单服，则不允许切换gs
    if (5 != var.GetCount())
    {
        return 0;
    }

    //     int nGameServerID = var.Int(0);
    //     const std::string& strAccount = var.String(1);
    //     const std::string& strRoleName = var.String(2);
    //     int nSceneIndex = var.Int(3);
    //     int nPlayerConfigIndex = var.Int(4);
    //
    //     _tagGTG_SWAP_GAMESERVER swapGS;
    //
    //     swapGS._nGameServerID = nGameServerID;
    //     strcpy(swapGS._szAccount, strAccount.c_str());
    //     strcpy(swapGS._szRoleName, strRoleName.c_str());
    //     swapGS._nSceneIndex = nSceneIndex;
    //     swapGS._nPlayerConfigIndex = nPlayerConfigIndex;
    //
    //     RakNet::BitStream oBitStream;
    //     swapGS.EnCode(oBitStream);
    //
    //     SendBitStream(&oBitStream);

    return 0;
}

int NFCGameServerNet_ClientModule::OnSwapGSProcess(const NFIPacket& msg)
{
    //查看此GSID是否正确
    //     _tagGTG_SWAP_GAMESERVER swapGameServer;
    //     RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    //     swapGameServer.DeCode(*pBitStream);
    //
    //     if (m_pGameLogicModule->GetGameID() == swapGameServer._nGameServerID)
    //     {
    //         if (m_pKernelModule->ExistContainer(swapGameServer._nSceneIndex))
    //         {
    //             //OK，可以接纳玩家
    //             NFCDataList valServerList;
    //             valServerList << swapGameServer._nGameServerID << swapGameServer._szAccount << swapGameServer._szRoleName << swapGameServer._nSceneIndex << swapGameServer._nPlayerConfigIndex;
    //             m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_CLIENT_SELECTROLE_ENTER, valServerList);
    //         }
    //     }

    return 0;
}

int NFCGameServerNet_ClientModule::OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
//     if (strClassName == "Player")
//     {
//         if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent
//             && m_pKernelModule->GetPropertyInt(self, "SceneID") > 0)
//         {
//             //被删除，也有可能是大厅角色，因此场景ID一定大于0才行
//             //删除他的连接和连接信息,包括世界服务器
//             _tagPT_KEY_BASE_MSG baseMsg;
//             NFMsg::WantToExitWorld xMsg;
// 
//             xMsg.set_world_id(0);
//             xMsg.set_account(m_pKernelModule->GetPropertyString(self, "Account"));
// 
//             if (xMsg.SerializeToString(&baseMsg.strSyncInfo))
//             {
//                 baseMsg._unMsgID = LTW_CLIENT_EXIT_WORLD;
//                 RakNet::BitStream oBitStream;
//                 baseMsg.EnCode(&oBitStream);
//                 SendBitStream(&oBitStream);
//             }
//         }
// 
//     }

    return 0;
}

int NFCGameServerNet_ClientModule::OnRecivePack( const NFIPacket& msg )
{
    return 0;
}

int NFCGameServerNet_ClientModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "Connected success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }
    else
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed or Got an error!", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }

    return 0;
}

void NFCGameServerNet_ClientModule::OnClientDisconnect( const int nAddress )
{
    UnRegister();
}

void NFCGameServerNet_ClientModule::OnClientConnected( const int nAddress )
{
    Register();
}