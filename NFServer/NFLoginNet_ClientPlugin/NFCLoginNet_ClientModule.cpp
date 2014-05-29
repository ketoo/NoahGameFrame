// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCLoginNet_ClientModule.h"
#include "NFLoginNet_ClientPlugin.h"

bool NFCLoginNet_ClientModule::Init()
{
	mstrConfigIdent = "LoginServer";
    mnWorldContainerID = -2;
	return true;
}

bool NFCLoginNet_ClientModule::Shut()
{
	return true;
}

bool NFCLoginNet_ClientModule::AfterInit()
{

	
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
	m_pLoginLogicModule = dynamic_cast<NFILoginLogicModule*>(pPluginManager->FindModule("NFCLoginLogicModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pLoginNet_ServerModule = dynamic_cast<NFILoginNet_ServerModule*>(pPluginManager->FindModule("NFCLoginNet_ServerModule"));
	
	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLoginLogicModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLoginNet_ServerModule);

	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER, this, &NFCLoginNet_ClientModule::OnSelectServerEvent);
	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_EXIT_SERVER, this, &NFCLoginNet_ClientModule::OnExitServerEvent);

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(this, &NFCLoginNet_ClientModule::OnRecivePack, &NFCLoginNet_ClientModule::OnSocketEvent);
	mnSocketFD = m_pNet->Initialization(strServerIP.c_str(), nServerPort);
	if (mnSocketFD < 0)
	{
		assert(0);
	}

	m_pKernelModule->CreateContainer(mnWorldContainerID, "");

	return true;
}

bool NFCLoginNet_ClientModule::BeforeShut()
{
    UnRegister();

	m_pNet->Final();
    delete m_pNet;
	m_pNet = NULL;

    return false;
}

int NFCLoginNet_ClientModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (4 != var.GetCount()
		|| !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT,
		VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
	{
		return -1;
	}

	int nServerID = var.IntVal(0);
	int nSenderAddress = var.IntVal(1);
	int nLoginID = var.IntVal(2);
	const std::string& strAccount = var.StringVal(3);

	NFMsg::ReqConnectWorld xData;
	xData.set_world_id(nServerID);
	xData.set_login_id(nLoginID);
	xData.set_sender_ip(nSenderAddress);
	xData.set_account(strAccount);

	SendMsg(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData, mnSocketFD);

	return 0;
}

bool NFCLoginNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCLoginNet_ClientModule::Register()
{
	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	NFMsg::ServerInfoReportList xMsg;
	NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

	pData->set_server_id(nServerID);
	pData->set_server_name(strName);
	pData->set_server_cur_count(0);
	pData->set_server_ip("");
	pData->set_server_port(0);
	pData->set_server_max_online(nMaxConnect);
	pData->set_server_state(NFMsg::EST_NARMAL);

	SendMsg(NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Register");
}

void NFCLoginNet_ClientModule::UnRegister()
{
	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	NFMsg::ServerInfoReportList xMsg;
	NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

	pData->set_server_id(nServerID);
	pData->set_server_name(strName);
	pData->set_server_cur_count(0);
	pData->set_server_ip("");
	pData->set_server_port(0);
	pData->set_server_max_online(nMaxConnect);
	pData->set_server_state(NFMsg::EST_MAINTEN);

	SendMsg(NFMsg::EGameMsgID::EGMI_LTM_LOGIN_UNREGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "UnRegister");
	//Execute(0.0f, 0.0f);
}

int NFCLoginNet_ClientModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
	//     _tagPT_KEY_BASE_MSG baseMsg;
	//     pMsgPacket->DeCode(&baseMsg);
	// 
	//     NFMsg::WantToConnectWorldResult xMsg;
	//     if (!xMsg.ParseFromString(baseMsg.strSyncInfo))
	//     {
	//         return 0;
	//     }
	// 
	//     NFCValueList var;
	//     var << xMsg.world_id()
	//         << xMsg.sender_ip()
	//         << xMsg.login_id()
	//         << xMsg.account()
	//         << xMsg.world_ip()
	//         << xMsg.world_port()
	//         << xMsg.world_key();
	// 
	//     m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, var);

	return 0;
}

int NFCLoginNet_ClientModule::OnExitServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (2 != var.GetCount()
		|| !var.TypeEx(VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_UNKNOWN))
	{
		return -1;
	}

	//     const std::string& strAccount = var.StringVal(0);
	//     int nWorldID = var.IntVal(1);
	// 
	//     _tagPT_KEY_BASE_MSG baseMsg;
	//     NFMsg::WantToExitWorld xMsg;
	// 
	//     xMsg.set_account(strAccount);
	//     xMsg.set_world_id(nWorldID);
	// 
	//     if (xMsg.SerializeToString(&baseMsg.strSyncInfo))
	//     {
	//         baseMsg._unMsgID = LTW_WANTTO_CONNECT_WORLD;
	//         RakNet::BitStream oBitStream;
	//         baseMsg.EnCode(&oBitStream);
	//         SendBitStream(&oBitStream);
	//     }

	return 0;
}

int NFCLoginNet_ClientModule::OnRecivePack(const NFIPacket& msg )
{
	//统一解包
	int nMsgID = msg.GetMsgHead().unMsgID;
	switch (nMsgID)
	{

	case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
		OnSelectServerResultProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_STS_NET_INFO:
		OnWorldInfoProcess(msg);
		break;

	default:
		printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
		break;
	}

	return 0;

}

int NFCLoginNet_ClientModule::OnSocketEvent( const uint16_t nSockIndex, const NF_NET_EVENT eEvent )
{
	if (eEvent & NF_NET_EVENT_EOF) 
	{
		printf("%d Connection closed.\n", nSockIndex);
	} 
	else if (eEvent & NF_NET_EVENT_ERROR) 
	{
		printf("%d Got an error on the connection: %s\n", nSockIndex,	strerror(errno));/*XXX win32*/
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		printf("%d read timeout: %s\n", nSockIndex, strerror(errno));/*XXX win32*/
	}
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
		printf("%d Got an unknow error on the connection: %s\n", nSockIndex,	strerror(errno));/*XXX win32*/
		Register();
	}

	return 0;
}

int NFCLoginNet_ClientModule::OnWorldInfoProcess( const NFIPacket& msg )
{
	int32_t nPlayerID = 0;	
	NFMsg::MultiObjectPropertyList xMsg;
	if (!Recive(msg, xMsg, nPlayerID))
	{
		return 0;
	}
	int nSize = xMsg.multi_player_property_size();
	for (int i = 0; i < nSize; ++i)
	{
		const NFMsg::ObjectPropertyList& xData = xMsg.multi_player_property(i);
		NFIObject* pObject = m_pKernelModule->GetObject(xData.player_id());
		if (!pObject)
		{
			pObject = m_pKernelModule->CreateObject(0, mnWorldContainerID, 0, "WorldServer", "", NFCValueList());
		}

		NFIPropertyManager* pProManager = pObject->GetPropertyManager();
		if (pProManager)
		{
			PropertyFormString(pProManager, xData);
		}
	}

	m_pLoginNet_ServerModule->SynWorldToClient();

	return 0;
}


