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

    m_pEventProcessModule->AddClassCallBack("WantToConnect", this, &NFCProxyServerNet_ServerModule::OnWantToConnectObjectEvent);
    m_pEventProcessModule->AddClassCallBack("ClientConnect", this, &NFCProxyServerNet_ServerModule::OnConnectObjectEvent);

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

int NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime(const NFIDENTID& self, const NFIValueList& var)
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
        //5秒客户端还不连接就删掉[还是一直等着他连接算了把]
        m_pKernelModule->AddHeartBeat( self, "HB_OnPlayerWantToConnect", this, &NFCProxyServerNet_ServerModule::HB_OnPlayerWantToConnect, NFCValueList(), 5.0f, 1 );
    }
    else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        //销毁
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
    //连接对象被销毁，T掉网络
    if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
    {
        //5秒客户端还不连接就删掉[也还有大厅的对象，因此不能这么搞]
        //m_pKernelModule->AddHeartBeat( self, "HB_OnConnectCheckTime", this, &NFCProxyServerNet_ServerModule::HB_OnConnectCheckTime, NFCValueList(), 5.0f, 1 );
    }
    else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        //销毁,client已做处理

        //T掉连接，并告诉gs1此帐号挂掉，如果ident>0,则再告诉其他gs
        //DisconnectOne();
        const std::string& strAccount = m_pKernelModule->QueryPropertyString(self, "Account");
        const std::string& strName = m_pKernelModule->QueryPropertyString(self, "RoleName");
        int nGameServerID = m_pKernelModule->QueryPropertyInt(self, "WorldID");

        //m_pProxyServerNet_ClientModule->Transpond(nGameServerID, &bitStream);
    }

    return 0;
}

int NFCProxyServerNet_ServerModule::OnConnectKeyProcess(const NFIPacket& msg)
{
    //删掉老心跳
//     NFCValueList valueMsgBody(strInfo.c_str(), ",");
//     if (valueMsgBody.GetCount() == 2)
//     {
//         const std::string& strAccount = valueMsgBody.StringVal(0);
//         const std::string& strConnectKey = valueMsgBody.StringVal(1);
// 
//         //验证帐号密钥对错
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
//         //mnWantToConnectContainer的对象暂时不删，以便如果客户端断线重连
//         RakNet::SystemAddress address = pMsgPacket->systemAddress;
//         NFCValueList varConnectObjectList;
//         int nCount = m_pKernelModule->GetObjectByProperty(mnConnectContainer, "Address", NFCValueList() << address.ToString(), varConnectObjectList);
//         if (1 == nCount)
//         {
//             NFIDENTID identConnectObject = varConnectObjectList.ObjectVal(0);
//             //这个连接属于哪个角色ID，角色名
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
    return 0;
}

int NFCProxyServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    //看他连接在哪个gs，然后转发

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

void NFCProxyServerNet_ServerModule::OnClientDisconnect( const uint32_t& nAddress )
{

}

void NFCProxyServerNet_ServerModule::OnClientConnected( const uint32_t& nAddress )
{
    NFCValueList arg;
    arg << "Address" << nAddress;
    m_pKernelModule->CreateObject(0, mnConnectContainer, 0, "ClientConnect", "", arg);
}

int NFCProxyServerNet_ServerModule::HB_OnPlayerWantToConnect( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    m_pKernelModule->DestroyObject( self );

    return 0;
}
