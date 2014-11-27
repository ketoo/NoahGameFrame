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
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFMessageDefine/NFMsgPreGame.pb.h"

bool NFCWorldNet_ClientModule::Init()
{
	mstrConfigIdent = "WorldServer";
    mfLastHBTime = 0.0f;
    mnSocketFD = -1;

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



	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

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
    KeepAlive(fLasFrametime);
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCWorldNet_ClientModule::KeepAlive(float fLasFrametime)
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

void NFCWorldNet_ClientModule::Register()
{
	const int nID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");

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
	const int nID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");


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
	NFIDENTID nPlayerID = 0;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    NFCDataList var;
    var << xMsg.world_id() << xMsg.sender_ip()  << xMsg.login_id() << xMsg.account();
    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER, var);

    return 0;
}

int NFCWorldNet_ClientModule::OnSelectServerResultsEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    if (var.GetCount() != 7
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
		return 0;
    }

    const int nWorldID = var.Int(0);
    const int nSenderAddress = var.Int(1);
    const int nLoginID = var.Int(2);
    const std::string& strAccount = var.String(3);
    const std::string& strWorldAddress = var.String(4);
    const int nPort = var.Int(5);
    const std::string& strKey = var.String(6);

    NFMsg::AckConnectWorldResult xMsg;

    xMsg.set_world_id(nWorldID);
    xMsg.set_login_id(nLoginID);
    xMsg.set_world_port(nPort);
    xMsg.set_sender_ip(nSenderAddress);
    xMsg.set_account(strAccount);
    xMsg.set_world_ip(strWorldAddress);
    xMsg.set_world_key(strKey);

	SendMsgPB(NFMsg::EGMI_ACK_CONNECT_WORLD, xMsg, mnSocketFD);

    return 0;
}

int NFCWorldNet_ClientModule::OnKickClientProcess(const NFIPacket& msg)
{
	NFIDENTID nPlayerID = 0;
	NFMsg::ReqKickFromWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    //T人,下线
    NFCDataList var;
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

int NFCWorldNet_ClientModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
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

void NFCWorldNet_ClientModule::OnClientDisconnect( const int nAddress )
{

}

void NFCWorldNet_ClientModule::OnClientConnected( const int nAddress )
{

}

bool NFCWorldNet_ClientModule::BeforeShut()
{
    UnRegister();

    return true;
}
