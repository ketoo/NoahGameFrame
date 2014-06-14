// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCProxyServerNet_ServerModule.h"
#include "NFProxyServerNet_ServerPlugin.h"

bool NFCProxyServerNet_ServerModule::Init()
{
    mnConnectContainer = -1;
    mnWantToConnectContainer = -2;
    mnRoleHallContainer = -3;

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
// 
//     m_pEventProcessModule->AddClassCallBack("WantToConnect", OnWantToConnectObjectEvent);
//     m_pEventProcessModule->AddClassCallBack("ClientConnect", OnConnectObjectEvent);


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

// void NFCProxyServerNet_ServerModule::CreateConnectObject(const std::string& strAddress)
// {
//     //ClientConnect
//     NFCValueList arg;
//     arg << "Address" << strAddress;
//     m_pKernelModule->CreateObject(0, mnConnectContainer, 0, "ClientConnect", "", arg);
// 
// }

// int NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime(const NFIDENTID& self, const NFIValueList& var)
// {
//     return 0;
// 
//     m_pKernelModule->DestroyObject(self);
//     return 0;
// }

int NFCProxyServerNet_ServerModule::GetServerPort()
{
    return 0;//GetPort();
}
// 
// int NFCProxyServerNet_ServerModule::OnWantToConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
// {
//     if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
//     {
//         //5��ͻ��˻������Ӿ�ɾ��[����һֱ�������������˰�]
//         //m_pKernelModule->AddHeartBeat( self, "HB_OnPlayerWantToConnect", HB_OnPlayerWantToConnect, NFCValueList(), 5.0f, 1 );
//     }
//     else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
//     {
//         //����
//     }
// 
//     return 0;
// }
// 
// 
// // int NFCGameServerNet_ServerModule::HB_OnPlayerWantToConnect( const NFIDENTID& self, const NFIValueList& var )
// // {
// //  std::string strIdent = NFIDENTID::ToString( self );
// //  strIdent.append( " " );
// //  strIdent.append( "HB_OnPlayerWantToConnect" );
// //  m_pKernelModule->LogInfo( strIdent.c_str() );
// //
// //  m_pKernelModule->DestroyObject( self );
// //  return 0;
// // }
// 
// int NFCProxyServerNet_ServerModule::OnConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
// {
//     //���Ӷ������٣�T������
//     if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
//     {
//         //5��ͻ��˻������Ӿ�ɾ��[Ҳ���д����Ķ�����˲�����ô��]
//         //m_pKernelModule->AddHeartBeat( self, "HB_OnConnectCheckTime", HB_OnConnectCheckTime, NFCValueList(), 5.0f, 1 );
//     }
//     else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
//     {
//         //����,client��������
// 
//         //T�����ӣ�������gs1���ʺŹҵ������ident>0,���ٸ�������gs
//         //DisconnectOne();
//         const std::string& strAccount = m_pKernelModule->QueryPropertyString(self, "Account");
//         const std::string& strName = m_pKernelModule->QueryPropertyString(self, "RoleName");
//         int nGameServerID = m_pKernelModule->QueryPropertyInt(self, "WorldID");
// 
//         //�ʺ������еģ����ǲ�һ����RoleName,��Ϊ���ܻ��ڽ�ɫ��������Ȼ���������Ҫ��һ������Ȼ����֪��������
//         //Ҳ�п����Ǵ�����ɫ�Ǹ���������ɾ����ɫ���ѣ���ô�����ǵ���ɾ���أ����ǽ�ɫ����ɾ����ɫ
//         //������RoleName,RoleIdent,WorldID�����Ӷ���û��ɫ����ֻ���ʺź͵�ַ
//         if (strAccount.length() > 0)
//         {
//             _tagPT_KEY_BASE_MSG baseMsg;
//             NFMsg::WantToExitWorld xMsg;
// 
//             //             xMsg.set_account(strAccount);
//             //             xMsg.set_world_id(strAccount);
//             //
//             //
//             //             if ( xMsg.SerializeToString( &baseMsg.strSyncInfo ) )
//             //             {
//             //                 baseMsg._unMsgID = CTG_LEAVE_GAME;
//             //                 RakNet::BitStream oBitStream;
//             //                 baseMsg.EnCode(&oBitStream);
//             //                 SendBitStream(&oBitStream);
//             //             }
//             //
//             //             _tagCTG_LEAVE_GAME leaveGame;
//             //             strcpy(leaveGame._szAccount, strAccount.c_str());
//             //             strcpy(leaveGame._szName, strName.c_str());
//             //             leaveGame._Ident = self.nData64;
//             //             RakNet::BitStream bitStream;
//             //             leaveGame.EnCode(bitStream);
//             //
//             //             if (self.nSerial > 1)
//             //             {
//             //                 //�ڼ��ž͸��߼���,��ɫ�����ģ�Ҳ����GameServerID��
//             //                 m_pProxyServerNet_ClientModule->Transpond(nGameServerID, &bitStream);
//             //             }
//             //             else
//             //             {
//             //                 //���gateserver���ˣ���ô�ڽ�ɫ�����Ľ�ɫ�Ծɴ��ڣ���Ϊû��������������Ϣ,��������´ε�¼����ɫ�б�ͻ��˿������ǿյ�
//             //                 //1��gs�����ϲ���Ҫ��������
//             //                 //                 if (strlen(pstrName) > 0)
//             //                 //                 {
//             //                 //                     m_pProxyServerNet_ClientModule->Transpond(1, &bitStream);
//             //                 //                 }
//             //             }
// 
//         }
//     }
// 
//     return 0;
// }
// 
// void NFCProxyServerNet_ServerModule::OnDisAccount(const std::string& strAddress)
// {
//     DestoryConnectObject(strAddress);
// }
// 
// void NFCProxyServerNet_ServerModule::DestoryConnectObject(const std::string& strAddress)
// {
//     std::string strAccount = NULL_STR;
// 
//     NFCValueList varObjectList;
//     int nCount = m_pKernelModule->GetObjectByProperty(mnConnectContainer, "Address", NFCValueList() << strAddress, varObjectList);
//     for (int i = 0; i < nCount; i++)
//     {
//         NFIDENTID ident = varObjectList.ObjectVal(i);
//         if (m_pKernelModule->GetObject(ident))
//         {
//             strAccount = m_pKernelModule->QueryPropertyString(ident, "Account");
// 
//             m_pKernelModule->DestroyObject(ident);
//         }
//     }
// 
//     if (strAccount.length() > 0)
//     {
//         NFCValueList varObjectList;
//         int nWantToConnecObjectCount = m_pKernelModule->GetObjectByProperty(mnWantToConnectContainer, "Account", NFCValueList() << strAccount, varObjectList);
//         if (1 == nWantToConnecObjectCount)
//         {
//             NFIDENTID identOldObject = varObjectList.ObjectVal(0);
//             m_pKernelModule->DestroyObject(identOldObject);
//         }
//     }
// 
// }
// 
// int NFCProxyServerNet_ServerModule::OnConnectKeyProcessTCP(RakNet::Packet* pMsgPacket, const std::string& strInfo)
// {
//     //ɾ��������
//     NFCValueList valueMsgBody(strInfo.c_str(), ",");
//     if (valueMsgBody.GetCount() == 2)
//     {
//         const std::string& strAccount = valueMsgBody.StringVal(0);
//         const std::string& strConnectKey = valueMsgBody.StringVal(1);
// 
//         //��֤�ʺ���Կ�Դ�
//         NFCValueList varObjectList;
//         NFCValueList varKeyList;
//         int nWantToConnecObjectCount = m_pKernelModule->GetObjectByProperty(mnWantToConnectContainer, "Account", NFCValueList() << strAccount, varObjectList);
//         int nWantToConnectKeyCount = m_pKernelModule->GetObjectByProperty(mnWantToConnectContainer, "ConnectKey", NFCValueList() << strConnectKey, varKeyList);
//         NFIDENTID identOldObject = varObjectList.ObjectVal(0);
//         NFIDENTID identKeyObject = varKeyList.ObjectVal(0);
// 
//         if (nWantToConnecObjectCount <= 0
//             || nWantToConnectKeyCount <= 0)
//         {
//             //ban this ip
//             //m_pKernelModule->LogStack();
//             return 0;
//         }
// 
//         //mnWantToConnectContainer�Ķ�����ʱ��ɾ���Ա�����ͻ��˶�������
//         RakNet::SystemAddress address = pMsgPacket->systemAddress;
//         NFCValueList varConnectObjectList;
//         int nCount = m_pKernelModule->GetObjectByProperty(mnConnectContainer, "Address", NFCValueList() << address.ToString(), varConnectObjectList);
//         if (1 == nCount)
//         {
//             NFIDENTID identConnectObject = varConnectObjectList.ObjectVal(0);
//             //������������ĸ���ɫID����ɫ��
//             //m_pKernelModule->SetPropertyObject( identConnectObject, "RoleIdent", 0);
//             //m_pKernelModule->SetPropertyString( identConnectObject, "RoleName", "");
//             m_pKernelModule->SetPropertyString(identConnectObject, "Account", strAccount);
//             m_pKernelModule->RemoveHeartBeat(identConnectObject, "HB_OnConnectCheckTime");
// 
//             //m_pProxyServerNet_ClientModule->
//             m_pEventProcessModule->DoEvent(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, NFCValueList() << strAccount);
//         }
//         else
//         {
//             //banip
//         }
//     }
//     return 0;
// }

int NFCProxyServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
	return 0;

}

int NFCProxyServerNet_ServerModule::OnSocketEvent( const uint16_t nSockIndex, const NF_NET_EVENT eEvent )
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

void NFCProxyServerNet_ServerModule::OnClientDisconnect( const uint16_t& nAddress )
{

}

void NFCProxyServerNet_ServerModule::OnClientConnected( const uint16_t& nAddress )
{

}
