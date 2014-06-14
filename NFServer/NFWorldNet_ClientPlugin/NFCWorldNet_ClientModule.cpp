// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCWorldNet_ClientModule.h"
#include "NFWorldNet_ClientPlugin.h"


bool NFCWorldNet_ClientModule::Init()
{

	mstrConfigIdent = "WorldServer";
    return true;
}

bool NFCWorldNet_ClientModule::Shut()
{
    

    m_pNet->Final();

    return true;
}

bool NFCWorldNet_ClientModule::AfterInit()
{
	
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pWorldLogicModule = dynamic_cast<NFIWorldLogicModule*>(pPluginManager->FindModule("NFCWorldLogicModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pWorldLogicModule);
	assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, this, &NFCWorldNet_ClientModule::OnSelectServerResultsEvent);



	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCWorldNet_ClientModule::OnRecivePack, &NFCWorldNet_ClientModule::OnSocketEvent);
	mnSocketFD = m_pNet->Initialization(strServerIP.c_str(), nServerPort);
	if (mnSocketFD < 0)
	{
		assert(0);
	}

	return true;
}


bool NFCWorldNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCWorldNet_ClientModule::Register()
{
	const int nID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");

    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nID);
    pData->set_server_name("世界服务器1");
    pData->set_server_cur_count(0);
    pData->set_server_ip("");
    pData->set_server_port(0);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_MTL_WORLD_REGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Register");
}

void NFCWorldNet_ClientModule::UnRegister()
{
	const int nID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");


    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nID);
    pData->set_server_name("世界服务器1");
    pData->set_server_cur_count(0);
    pData->set_server_ip("");
    pData->set_server_port(0);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_MTL_WORLD_UNREGISTERED, xMsg, mnSocketFD);

    Execute(0.0f, 0.0f);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "UnRegister");

}

void NFCWorldNet_ClientModule::RefreshWorldInfo()
{

}

int NFCWorldNet_ClientModule::OnSelectServerProcess(const NFIPacket& msg)
{
	int32_t nPlayerID = 0;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    NFCValueList var;
    var << xMsg.world_id() << xMsg.sender_ip()  << xMsg.login_id() << xMsg.account();
    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER, var);

    return 0;
}

int NFCWorldNet_ClientModule::OnSelectServerResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (var.GetCount() != 7
        || var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT,
		VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_STRING,
		VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
		return 0;
    }

    const int nWorldID = var.IntVal(0);
    const int nSenderAddress = var.IntVal(1);
    const int nLoginID = var.IntVal(2);
    const std::string& strAccount = var.StringVal(3);
    const std::string& strWorldAddress = var.StringVal(4);
    const int nPort = var.IntVal(5);
    const std::string& strKey = var.StringVal(6);

    NFMsg::AckConnectWorldResult xMsg;

    xMsg.set_world_id(nWorldID);
    xMsg.set_login_id(nLoginID);
    xMsg.set_world_port(nPort);
    xMsg.set_sender_ip(nSenderAddress);
    xMsg.set_account(strAccount);
    xMsg.set_world_ip(strWorldAddress);
    xMsg.set_world_key(strKey);

	SendMsgPB(NFMsg::EGMI_ACK_CONNECT_WORLD, xMsg, 0);

    return 0;
}

int NFCWorldNet_ClientModule::OnKickClientProcess(const NFIPacket& msg)
{
	int32_t nPlayerID = 0;
	NFMsg::ReqKickFromWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    //T人,下线
    NFCValueList var;
    var << xMsg.world_id() << xMsg.account();
    m_pEventProcessModule->DoEvent(0, NFED_ON_KICK_FROM_SERVER, var);
    return 0;
}

int NFCWorldNet_ClientModule::OnRecivePack( const NFIPacket& msg )
{
	int nMsgID = msg.GetMsgHead()->GetMsgID();
	switch (nMsgID)
	{
	case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD:
		OnSelectServerProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_KICK_CLIENT_INWORLD:
		OnKickClientProcess(msg);
		break;

	default:
		printf("NFNet || 非法消息:unMsgID=%d\n", msg.GetMsgHead()->GetMsgID());
		break;
	}

	return 0;
}

int NFCWorldNet_ClientModule::OnSocketEvent( const uint16_t nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register();
    }

	return 0;
}

void NFCWorldNet_ClientModule::OnClientDisconnect( const uint16_t& nAddress )
{

}

void NFCWorldNet_ClientModule::OnClientConnected( const uint16_t& nAddress )
{

}

bool NFCWorldNet_ClientModule::BeforeShut()
{
    UnRegister();

    return true;
}
