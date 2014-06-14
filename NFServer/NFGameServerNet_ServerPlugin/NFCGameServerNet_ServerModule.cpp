// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerNet_ServerModule.h"
#include "NFComm\NFPluginModule\NFILogicModule.h"

bool NFCGameServerNet_ServerModule::Init()
{
    mnRoleHallContainer = -3;
    mnProxyContainer = -4;
    mstrConfigIdent = "GameServer";
    
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>(pPluginManager->FindModule("NFCSceneProcessModule"));
    m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>(pPluginManager->FindModule("NFCGameLogicModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pGameLogicModule);
    assert(NULL != m_pElementInfoModule);


    return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{

//     m_pKernelModule->LogInfo(" -1 ConnectContainer ");
//     m_pKernelModule->LogInfo(" -2 WantToConnectContainer ");

//     m_pKernelModule->ResgisterCommonClassEvent(OnClassCommonEvent);
//     m_pKernelModule->ResgisterCommonPropertyEvent(OnPropertyCommonEvent);
//     m_pKernelModule->ResgisterCommonRecordEvent(OnRecordCommonEvent);
// 
//     m_pEventProcessModule->AddClassCallBack("Player", OnObjectClassEvent);

    //  m_pEventProcessModule->AddClassCallBack("AttackNPC", OnClassObjectEvent);
    //  m_pEventProcessModule->AddClassCallBack("Pet", OnClassObjectEvent);

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ServerModule::OnRecivePack, &NFCGameServerNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}


    return true;
}

bool NFCGameServerNet_ServerModule::Shut()
{

    return true;
}

bool NFCGameServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCGameServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    // TOCHECK
    //NFIActorMessage xActorMsg;

    //xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG;
    //xActorMsg.nSubMsgID = msg.GetMsgHead()->GetMsgID();
    //xActorMsg.data.assign(msg.GetData(), msg.GetDataLen());

    ////根据用户基础信息得到目标actor地址,看自己想怎么管理和优化
    //pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());
    return true;
}

int NFCGameServerNet_ServerModule::OnSocketEvent( const uint16_t nSockIndex, const NF_NET_EVENT eEvent )
{
	if (eEvent & NF_NET_EVENT_EOF) 
	{
		//printf("%d Connection closed.\n", nSockIndex);
		OnClientDisconnect(nSockIndex);
	} 
	else if (eEvent & NF_NET_EVENT_ERROR) 
	{
		//printf("%d Got an error on the connection: %s\n", pObject->GetFd(),	strerror(errno));/*XXX win32*/
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		//printf("%d read timeout: %s\n", pObject->GetFd(), strerror(errno));/*XXX win32*/
		OnClientDisconnect(nSockIndex);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		OnClientConnected(nSockIndex);
	}
	else
	{
		//printf("%d Got an unknow error on the connection: %s\n", pObject->GetFd(),	strerror(errno));/*XXX win32*/
		OnClientDisconnect(nSockIndex);
	}

	return true;
}

void NFCGameServerNet_ServerModule::OnClientDisconnect( const uint16_t& nAddress )
{

}

void NFCGameServerNet_ServerModule::OnClientConnected( const uint16_t& nAddress )
{
	NFCValueList varList;
	m_pKernelModule->GetObjectByProperty(mnProxyContainer, "FD", NFCValueList() << nAddress, varList);
	if (varList.GetCount() > 0)
	{
		m_pKernelModule->DestroyObject(varList.ObjectVal(0));		
	}
}
