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
    mnClientConnectContainer = -1;
    mnWorldContainerID = -2;
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

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLoginLogicModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGIN_RESULTS, this, &NFCLoginNet_ServerModule::OnLoginResultsEvent);
	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGOUT_RESULTS, this, &NFCLoginNet_ServerModule::OnLogOutResultsEvent);
	m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, this, &NFCLoginNet_ServerModule::OnSelectWorldResultsEvent);

    //std::function<void ()> fp = std::bind(&A::A_fun,this, ,std::placeholders::_1);  
    //fp();

	m_pEventProcessModule->AddClassCallBack("WorldServer", this, &NFCLoginNet_ServerModule::OnWorldServerObjectEvent);

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


	m_pKernelModule->CreateContainer(mnClientConnectContainer, "");

	return true;
}

int NFCLoginNet_ServerModule::OnLoginResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (3 != var.GetCount()
		|| !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
	{
		return -1;
	}

	int nState = var.IntVal(0);
	uint16_t unAddress = var.IntVal(1);
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

	NFCValueList varList;
	GetConnectObjectByAddress(unAddress, varList);
	if (varList.GetCount() != 1)
	{
		//他已掉线或者被T了
		return 0;
	}

	NFIDENTID ident = varList.ObjectVal(0);
	//返回的帐号和申请的是否一样
	//同一帐号试图多次登录,挤掉以前的，进入游戏世界的时候才T，不进入同一个区无所谓
	const std::string& strOldReqAccount = m_pKernelModule->QueryPropertyString(ident, PROPERTY_ACCOUNT);
	if (!strOldReqAccount.empty() && strOldReqAccount != strAccount)
	{
		//连续快速输入了几次帐号申请登录
		return 0;
	}


	//把服务器列表广播下去
	//SynWorldToClient(unAddress);
	NFMsg::AckEventResult xData;
	xData.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xData, unAddress);

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,ident, "Login successed :", strAccount.c_str());

	return 0;
}

int NFCLoginNet_ServerModule::OnLogOutResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (3 != var.GetCount()
		|| !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
	{
		return -1;
	}

	uint16_t unAddress = var.IntVal(0);
	const std::string& strAccount = var.StringVal(1);
	const std::string& strPassword = var.StringVal(2);


	return 0;
}

int NFCLoginNet_ServerModule::OnSelectWorldResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
	if (7 != var.GetCount()
		|| !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING,
		VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
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

int NFCLoginNet_ServerModule::OnWorldServerObjectEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//delete object
		NFCValueList valueAddress;
		//SynToClientWorldEndSpring(self, valueAddress);

		return 0;
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{

		NFIObject* pObject = m_pKernelModule->GetObject(self);

	}

	return 0;
}

int NFCLoginNet_ServerModule::GetConnectObjectByAddress(const int nAddress, NFIValueList& varObjectList)
{
	return m_pKernelModule->GetObjectByProperty(mnClientConnectContainer, "FD", NFCValueList() << nAddress, varObjectList);
}



void NFCLoginNet_ServerModule::OnClientConnected(const uint16_t& nAddress)
{
	NFCValueList varList;
	GetConnectObjectByAddress(nAddress, varList);
	if (varList.GetCount() == 0)
	{
		NFCValueList arg;
		arg << "FD" <<  nAddress;
		NFIObject* pObject = m_pKernelModule->CreateObject(0, mnClientConnectContainer, 0, "ClientConnect", "", arg);
		if (pObject)
		{
			//30秒内不验证就T掉
			//pObject->AddHeartBeat()
		}
	}
}

void NFCLoginNet_ServerModule::OnClientDisconnect(const uint16_t& nAddress)
{
	NFCValueList varList;
	GetConnectObjectByAddress(nAddress, varList);
	if (varList.GetCount() == 1)
	{
		//还没验证就掉线了
		m_pKernelModule->DestroyObject(varList.ObjectVal(0));
	}



	//assert(0);
}

int NFCLoginNet_ServerModule::OnLoginProcess( const NFIPacket& msg )
{
	int32_t nPlayerID = 0;
	NFMsg::ReqAccountLogin xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	NFCValueList varList;
	GetConnectObjectByAddress(msg.GetFd(), varList);
	if (varList.GetCount() <= 0)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, 0, "Account has Logined:", xMsg.account());
		return 0;
	}

	NFIDENTID ident = varList.ObjectVal(0);
	int nVerified = m_pKernelModule->QueryPropertyInt(ident, PROPERTY_VERIFIED);
	if (nVerified > 0)
	{
		//已经登录过了却还在发
		//assert
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, 0, "Account has Logined:", xMsg.account());
		return 0;
	}

	NFCValueList val;
	val << msg.GetFd() << xMsg.account() << xMsg.password();
	m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGIN, val);
	return 0;
}

int NFCLoginNet_ServerModule::OnLogoutProcess( const NFIPacket& msg )
{
	int32_t nPlayerID = 0;
	NFMsg::ReqAccountLogout xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	//1:account 2:logoutInfomation
	// 	xMsg.account()
	// 	    NFCValueList val;
	// 	    val << xMsg.account() << msg.GetMsgHead().
	// 	    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGOUT, val);

	//发消息给WORLD，T掉此帐号，T掉一个区的吧，因此，还得告诉是T掉哪个区，还是只要有都T掉

	return 0;
}

int NFCLoginNet_ServerModule::OnSelectWorldProcess( const NFIPacket& msg )
{
	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	int32_t nPlayerID = 0;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	NFCValueList varList;
	GetConnectObjectByAddress(msg.GetFd(), varList);
	if (varList.GetCount() != 1)
	{
		//他已掉线或者被T了,非法
		return 0;
	}

	const std::string& strAccount = m_pKernelModule->QueryPropertyString(varList.ObjectVal(0), PROPERTY_ACCOUNT);
	if (!strAccount.empty())
	{
		return 0;
	}
	
	NFCValueList val;
	val << xMsg.world_id() << msg.GetFd() << nServerID << strAccount;
	m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER, val);

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
		OnLogoutProcess(msg);
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

int NFCLoginNet_ServerModule::OnSocketEvent( const uint16_t nSockIndex, const NF_NET_EVENT eEvent )
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

	return 0;	
}

void NFCLoginNet_ServerModule::SynWorldToClient()
{

	NFCValueList varLoginObjectList;
	m_pKernelModule->GetContainerOnLineList(mnClientConnectContainer, varLoginObjectList);
	if (varLoginObjectList.GetCount() > 0)
	{
		for (int i = 0; i < varLoginObjectList.GetCount(); i++)
		{
			NFIDENTID ident = varLoginObjectList.ObjectVal(i);
			int nFD = m_pKernelModule->QueryPropertyInt(ident, "FD");
			SynWorldToClient(nFD);
		}
	}
}

void NFCLoginNet_ServerModule::SynWorldToClient( const int nFD )
{
	NFCValueList varObjectList;
	m_pKernelModule->GetContainerOnLineList(mnWorldContainerID, varObjectList);


	if (varObjectList.GetCount() > 0)
	{
		NFMsg::AckServerList xData;
		xData.set_type(NFMsg::RSLT_WORLD_SERVER);

		for (int i = 0; i < varObjectList.GetCount(); i++)
		{
			NFIDENTID ident = varObjectList.ObjectVal(i);

			NFIObject* pObject = m_pKernelModule->GetObject(ident);
			if (pObject)
			{
				NFMsg::ServerInfo* pServerInfo = xData.add_info();
				int nServerID = pObject->QueryPropertyInt("ServerID");
				int nState = pObject->QueryPropertyInt("State");
				const std::string& strName = pObject->QueryPropertyString("Name");
				pServerInfo->set_name(strName);
				pServerInfo->set_status((NFMsg::EServerState)nState);
				pServerInfo->set_server_id(nServerID);
				pServerInfo->set_wait_count(0);
			}
		}

		SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLS_LIST, xData, nFD);

	}
}

int NFCLoginNet_ServerModule::OnViewWorldProcess( const NFIPacket& msg )
{
	int32_t nPlayerID = 0;
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


