// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCLoginNet_ServerModule.h"
#include "NFLoginNet_ServerPlugin.h"

const std::string PROPERTY_ACCOUNT = "Account";
const std::string PROPERTY_VERIFIED = "Verified";

bool NFCLoginNet_ServerModule::Init()
{
	mstrConfigIdent = "LoginServer";
	return true;
}

bool NFCLoginNet_ServerModule::Shut()
{
	return true;
}

bool NFCLoginNet_ServerModule::BeforeShut()
{
    m_pNet->Final();
    delete m_pNet;
    m_pNet = NULL;

	return true;
}

bool NFCLoginNet_ServerModule::AfterInit()
{
	
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
	m_pLoginLogicModule = dynamic_cast<NFILoginLogicModule*>(pPluginManager->FindModule("NFCLoginLogicModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLoginNet_ClientModule = dynamic_cast<NFILoginNet_ClientModule*>(pPluginManager->FindModule("NFCLoginNet_ClientModule"));
    

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLoginLogicModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLoginNet_ClientModule);

	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGIN_RESULTS, this, &NFCLoginNet_ServerModule::OnLoginResultsEvent);
	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, this, &NFCLoginNet_ServerModule::OnSelectWorldResultsEvent);

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCLoginNet_ServerModule::OnRecivePack, &NFCLoginNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}

	return true;
}

int NFCLoginNet_ServerModule::OnLoginResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (3 != var.GetCount()
		|| !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
	{
		return -1;
	}

	int nState = var.IntVal(0);
	int unAddress = var.IntVal(1);
	const std::string& strAccount = var.StringVal(2);

	if (0 != nState)
	{
		//此帐号密码错误或者被封号
		//登录失败
		NFMsg::AckEventResult xMsg;
		xMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);

		SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xMsg, unAddress);

		return 0;
	}

    NetObject* pNetObject = GetNet()->GetNetObject(unAddress);
    if (pNetObject)
    {
        //记录他登录过
        pNetObject->SetUserData(1);
        pNetObject->SetUserStrData(strAccount);
    }

	//把服务器列表广播下去
	
	NFMsg::AckEventResult xData;
	xData.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xData, unAddress);

    //SynWorldToClient(unAddress);

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, unAddress, "Login successed :", strAccount.c_str());

	return 0;
}


int NFCLoginNet_ServerModule::OnSelectWorldResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (7 != var.GetCount()
		|| !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING,
		TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
	{
		return -1;
	}

	const int nWorldID = var.IntVal(0);
	const uint16_t nSenderAddress = var.IntVal(1);
	const int nLoginID = var.IntVal(2);
	const std::string& strAccount = var.StringVal(3);
	const std::string& strWorldAddress = var.StringVal(4);
	int nPort = var.IntVal(5);
	const std::string& strWorldKey = var.StringVal(6);

	NFMsg::AckConnectWorldResult xMsg;
	xMsg.set_world_id(nWorldID);
	xMsg.set_sender_ip(nSenderAddress);
	xMsg.set_login_id(nLoginID);
	xMsg.set_account(strAccount);
	xMsg.set_world_ip(strWorldAddress);
	xMsg.set_world_port(nPort);
	xMsg.set_world_key(strWorldKey);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, nSenderAddress);

	return 0;
}

bool NFCLoginNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCLoginNet_ServerModule::OnClientConnected(const int nAddress)
{

}

void NFCLoginNet_ServerModule::OnClientDisconnect(const int nAddress)
{

}

int NFCLoginNet_ServerModule::OnLoginProcess( const NFIPacket& msg )
{
	int64_t nPlayerID = 0;
	NFMsg::ReqAccountLogin xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}
    
    NetObject* pNetObject = GetNet()->GetNetObject(msg.GetFd());
    if (pNetObject)
    {
        //还没有登录过
        if (pNetObject->GetUserData() == 0)
        {
            NFCValueList val;
            val << msg.GetFd() << xMsg.account() << xMsg.password();
            m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGIN, val);
        }
    }
	
	return 0;
}

int NFCLoginNet_ServerModule::OnSelectWorldProcess( const NFIPacket& msg )
{
    const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
//     const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
//     const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
//     const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
//     const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
//     const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	int64_t nPlayerID = 0;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    NetObject* pNetObject = GetNet()->GetNetObject(msg.GetFd());
    if (pNetObject)
    {
        //登录过
        if (pNetObject->GetUserData() > 0)
        {
            NFCValueList val;
            val << xMsg.world_id() << msg.GetFd() << nServerID << pNetObject->GetUserStrData().c_str();
            m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER, val);
        }
    }

	return 0;
}

int NFCLoginNet_ServerModule::OnRecivePack(const NFIPacket& msg )
{
	//统一解包
	int nMsgID = msg.GetMsgHead()->GetMsgID();
	switch (nMsgID)
	{

	case NFMsg::EGameMsgID::EGMI_REQ_LOGIN:
		OnLoginProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_LOGOUT:
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD:
		OnSelectWorldProcess(msg);
		break;
		
	case NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST:
		OnViewWorldProcess(msg);
		break;

		// 	case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
		// 		OnObjectPropertyIntProcess(msg);
		// 		break;



	default:
		printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
		break;
	}
	return 0;
}

int NFCLoginNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
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

void NFCLoginNet_ServerModule::SynWorldToClient( const int nFD )
{
    NFMsg::AckServerList xData;
    xData.set_type(NFMsg::RSLT_WORLD_SERVER);

    NFMap<int, NFMsg::ServerInfoReport>* pWorldMap = m_pLoginNet_ClientModule->GetWorldMap();
    NFMsg::ServerInfoReport* pWorldData =  pWorldMap->First();
    while (pWorldData)
    {
        NFMsg::ServerInfo* pServerInfo = xData.add_info();

        pServerInfo->set_name(pWorldData->server_name());
        pServerInfo->set_status(pWorldData->server_state());
        pServerInfo->set_server_id(pWorldData->server_id());
        pServerInfo->set_wait_count(0);

        pWorldData = pWorldMap->Next();
    }

    SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLD_LIST, xData, nFD);
}

int NFCLoginNet_ServerModule::OnViewWorldProcess( const NFIPacket& msg )
{
	int64_t nPlayerID = 0;
	NFMsg::ReqServerList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	if (xMsg.type() == NFMsg::RSLT_WORLD_SERVER)
	{
		SynWorldToClient(msg.GetFd());
	}

	return 0;
}


