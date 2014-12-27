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
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFCLoginNet_ClientModule::Init()
{
	mstrConfigIdent = "LoginServer";
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

    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_CLIENT_SELECT_SERVER, this, &NFCLoginNet_ClientModule::OnSelectServerEvent);

    const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
    const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
    const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
    const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
    const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");


	Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCLoginNet_ClientModule::OnRecivePack, &NFCLoginNet_ClientModule::OnSocketEvent, strServerIP.c_str(), nServerPort);

    return true;
}

bool NFCLoginNet_ClientModule::BeforeShut()
{
    UnRegister();

// 	m_pNet->Final();
//     delete m_pNet;
// 	m_pNet = NULL;

    return false;
}

int NFCLoginNet_ClientModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
	if (4 != var.GetCount()
		|| !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
	{
		return -1;
	}

	int nServerID = var.Int(0);
	int nSenderAddress = var.Int(1);
	int nLoginID = var.Int(2);
	const std::string& strAccount = var.String(3);

	NFMsg::ReqConnectWorld xData;
	xData.set_world_id(nServerID);
	xData.set_login_id(nLoginID);
	xData.set_sender_ip(nSenderAddress);
	xData.set_account(strAccount);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData, GetNet()->FD());

	return 0;
}

bool NFCLoginNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return NFINetModule::Execute(fLasFrametime, fStartedTime);
}

void NFCLoginNet_ClientModule::Register()
{
	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

	NFMsg::ServerInfoReportList xMsg;
	NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

	pData->set_server_id(nServerID);
	pData->set_server_name(strName);
	pData->set_server_cur_count(0);
	pData->set_server_ip("");
	pData->set_server_port(0);
	pData->set_server_max_online(nMaxConnect);
	pData->set_server_state(NFMsg::EST_NARMAL);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED, xMsg, GetNet()->FD());

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Register");
}

void NFCLoginNet_ClientModule::UnRegister()
{
	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

	NFMsg::ServerInfoReportList xMsg;
	NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

	pData->set_server_id(nServerID);
	pData->set_server_name(strName);
	pData->set_server_cur_count(0);
	pData->set_server_ip("");
	pData->set_server_port(0);
	pData->set_server_max_online(nMaxConnect);
	pData->set_server_state(NFMsg::EST_MAINTEN);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_LTM_LOGIN_UNREGISTERED, xMsg, GetNet()->FD());

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "UnRegister");
	//Execute(0.0f, 0.0f);
}

int NFCLoginNet_ClientModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;	
    NFMsg::AckConnectWorldResult xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    NFCDataList var;
    var << xMsg.world_id()
        << xMsg.sender_ip()
        << xMsg.login_id()
        << xMsg.account()
        << xMsg.world_ip()
        << xMsg.world_port()
        << xMsg.world_key();

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_CLIENT_SELECT_SERVER_RESULTS, var);

	return 0;
}

int NFCLoginNet_ClientModule::OnRecivePack(const NFIPacket& msg )
{
    //统一解包
    int nMsgID = msg.GetMsgHead()->GetMsgID();
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

int NFCLoginNet_ClientModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register();
    }

	return 0;
}

int NFCLoginNet_ClientModule::OnWorldInfoProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID ;	
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	int nSize = xMsg.server_list_size();
	for (int i = 0; i < nSize; ++i)
	{
		const NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        std::shared_ptr<NFMsg::ServerInfoReport> pServerData = mWorldMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = std::shared_ptr<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
            *pServerData = *pData;

            mWorldMap.AddElement(pData->server_id(), pServerData);
        }
      
	}

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSize), "", "WorldInfo");

	return 0;
}

NFMapEx<int, NFMsg::ServerInfoReport>* NFCLoginNet_ClientModule::GetWorldMap()
{
    return &mWorldMap;
}


