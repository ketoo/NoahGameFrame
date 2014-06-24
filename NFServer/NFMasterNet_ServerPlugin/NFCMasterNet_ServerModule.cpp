// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCMasterNet_ServerModule.h"
#include "NFMasterNet_ServerPlugin.h"

bool NFCMasterNet_ServerModule::Init()
{
    mnWorldContainerID = - 2;
    mnLoginContainerID = -1;

	mstrConfigIdent = "MasterServer";
	return true;
}

bool NFCMasterNet_ServerModule::Shut()
{
	return true;
}

int NFCMasterNet_ServerModule::OnWorldRegisteredProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetWorldObject(pData->server_id(), varList);
        if (varList.GetCount() != 0)
        {
            NFIObject* pObject = m_pKernelModule->GetObject(varList.ObjectVal(i));
            if (pObject)
            {
                pObject->SetPropertyInt("FD", msg.GetFd());
                pObject->SetPropertyInt("State", pData->server_state());
                pObject->SetPropertyInt("ServerID", pData->server_id());
                pObject->SetPropertyString("Name", pData->server_name());
                pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
                pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
            }
            return 0;
        }

        //默认都0号分组
        NFCValueList arg;
        arg << "FD" << msg.GetFd();
        NFIObject* pObject = m_pKernelModule->CreateObject(0, mnWorldContainerID, 0, "WorldServer", "", arg);
        if (pObject)
        {
			pObject->SetPropertyInt("FD", msg.GetFd());
			pObject->SetPropertyInt("State", pData->server_state());
            pObject->SetPropertyInt("ServerID", pData->server_id());
            pObject->SetPropertyString("Name", pData->server_name());
            pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
            pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
        }

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "WorldRegistered");
    }
	

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetWorldObject(pData->server_id(), varList);
        if (varList.GetCount() <= 0)
        {
            return 0;
        }

        m_pKernelModule->DestroyObject(varList.ObjectVal(0));

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "WorldUnRegistered");

    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetWorldObject(pData->server_id(), varList);
        if (varList.GetCount() != 1)
        {
            return 0;
        }

        NFIDENTID ident = varList.ObjectVal(0);
		NFIObject* pObject = m_pKernelModule->GetObject(ident);
		if (pObject)
		{
			pObject->SetPropertyInt("State", pData->server_state());
			pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
			pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
		}

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "RefreshWorldInfo");

    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnLoginRegisteredProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

	    NFCValueList varList;
	    GetLoginObject(pData->server_id(), varList);
	    if (varList.GetCount() != 0)
	    {
		    return 0;
	    }

	    //默认都0号分组
	    NFCValueList arg;
	    arg << "FD" << msg.GetFd();
	    NFIObject* pObject = m_pKernelModule->CreateObject(0, mnLoginContainerID, 0, "LoginServer", "", arg);
	    if (pObject)
	    {
			pObject->SetPropertyInt("FD", msg.GetFd());
			pObject->SetPropertyInt("State", pData->server_state());
			pObject->SetPropertyInt("ServerID", pData->server_id());
			pObject->SetPropertyString("Name", pData->server_name());
			pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
			pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
	    }

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "LoginRegistered");
    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetLoginObject(pData->server_id(), varList);
        if (varList.GetCount() != 1)
        {
            return 0;
        }

        m_pKernelModule->DestroyObject(varList.ObjectVal(0));

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "LoginUnRegistered");

    }


	return 0;
}

int NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NFCValueList varList;
        GetLoginObject(pData->server_id(), varList);
        if (varList.GetCount() != 1)
        {
            return 0;
        }

		NFIDENTID ident = varList.ObjectVal(0);
		NFIObject* pObject = m_pKernelModule->GetObject(ident);
		if (pObject)
		{
			pObject->SetPropertyInt("State", pData->server_state());
			pObject->SetPropertyInt("MaxConnect", pData->server_max_online());
			pObject->SetPropertyInt("OnlineCount", pData->server_cur_count());
		}

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "RefreshLoginInfo");

    }

	return 0;
}

int NFCMasterNet_ServerModule::OnSelectWorldProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	NFCValueList varList;
	GetWorldObject(xMsg.world_id(), varList);
	if (varList.GetCount() != 1)
	{
		return 0;
	}

	//转发送到世界服务器
	uint16_t nSocket = m_pKernelModule->QueryPropertyInt(varList.ObjectVal(0), "FD");
	SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg, nSocket);

	return 0;
}

bool NFCMasterNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	m_pNet->Execute(fLasFrametime, fStartedTime);

	return true;
}

int NFCMasterNet_ServerModule::OnWorldServerObjectEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
    //std::cout << "AddClassCallBack：" << strClassName << " EventID:" << eClassEvent << std::endl;
	return 0;
}

int NFCMasterNet_ServerModule::OnWorldServerObjectPropertyEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar)
{
    //std::cout << "AddPropertyCallBack：" << strPropertyName << std::endl;
	//统计全世界总在线多少人等
	if (oldVar.GetCount() <= 0)
	{
		return 0;
	}

	return 0;
}

int NFCMasterNet_ServerModule::GetWorldObject(const int nWorldID, NFIValueList& varObjectList)
{
	return m_pKernelModule->GetObjectByProperty(mnWorldContainerID, "ServerID", NFCValueList() << nWorldID, varObjectList);
}

int NFCMasterNet_ServerModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
	NFMsg::AckConnectWorldResult xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

	NFCValueList varList;
	GetLoginObject(xMsg.login_id(), varList);
	if (varList.GetCount() != 1)
	{
		return 0;
	}

	//转发送到登录服务器
	uint16_t nSocket = m_pKernelModule->QueryPropertyInt(varList.ObjectVal(0), "FD");
	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, nSocket);

	return 0;
}

int NFCMasterNet_ServerModule::GetLoginObject(const int nLoginID, NFIValueList& varObjectList)
{
	return m_pKernelModule->GetObjectByProperty(mnLoginContainerID, "ServerID", NFCValueList() << nLoginID, varObjectList);
}

bool NFCMasterNet_ServerModule::AfterInit()
{
	

	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	
	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

    m_pEventProcessModule->AddClassCallBack("WorldServer", this, &NFCMasterNet_ServerModule::OnWorldServerObjectEvent);
    //m_pEventProcessModule->AddClassCallBack("LoginServer", this, &NFCMasterNet_ServerModule::OnWorldServerObjectEvent);
    //m_pEventProcessModule->AddEventCallBack(0, 1, this, &NFCMasterNet_ServerModule::OnTestEvent);
    //m_pEventProcessModule->DoEvent(0, 1, NFCValueList());

	//////////////////////////////////////////////////////////////////////////



	m_pKernelModule->CreateContainer(mnLoginContainerID, "");
	m_pKernelModule->CreateContainer(mnWorldContainerID, "");


	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCMasterNet_ServerModule::OnRecivePack, &NFCMasterNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}

	return true;
}

//int NFCMasterNet_ServerModule::OnTestEvent(const NFIDENTID& self, const int nEventID, const NFIValueList& var)
//{
//    std::cout << "AddEventCallBack：" << self.nData64 << std::endl;
//    return 0;
//}

int NFCMasterNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
	int nMsgID = msg.GetMsgHead()->GetMsgID();
	switch (nMsgID)
	{
	case NFMsg::EGameMsgID::EGMI_UNKNOW:
		break;

	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_REGISTERED:
		OnWorldRegisteredProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_UNREGISTERED:
		OnWorldUnRegisteredProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_REFRESH:
		OnRefreshWorldInfoProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED:
		OnLoginRegisteredProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_UNREGISTERED:
		OnLoginUnRegisteredProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REFRESH:
		OnRefreshLoginInfoProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD:
		OnSelectWorldProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
		OnSelectServerResultProcess(msg);
		break;

	default:
		printf("NFNet || 非法消息:unMsgID=%d\n", msg.GetMsgHead()->GetMsgID());
		break;
	}

	return 0;

}

int NFCMasterNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
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

void NFCMasterNet_ServerModule::OnClientDisconnect( const int nAddress )
{
	//不管是login还是world都要找出来,替他反注册
	NFCValueList varList;
	m_pKernelModule->GetObjectByProperty(mnWorldContainerID, "FD", NFCValueList() << nAddress, varList);
	if (varList.GetCount() > 0)
	{
		m_pKernelModule->SetPropertyInt(varList.ObjectVal(0), "State", NFMsg::EST_CRASH);
		SynWorldToLogin();
	}

	varList.Clear();
	m_pKernelModule->GetObjectByProperty(mnLoginContainerID, "FD", NFCValueList() << nAddress, varList);
	if (varList.GetCount() > 0)
	{
		m_pKernelModule->DestroyObject(varList.ObjectVal(0));
	}
}

void NFCMasterNet_ServerModule::OnClientConnected( const int nAddress )
{
	//连接上来啥都不做
}

void NFCMasterNet_ServerModule::SynWorldToLogin()
{
	NFCValueList varObjectList;
	m_pKernelModule->GetContainerOnLineList(mnWorldContainerID, varObjectList);


	if (varObjectList.GetCount() > 0)
	{
		NFList<NFIPropertyManager*> list;

		for (int i = 0; i < varObjectList.GetCount(); i++)
		{
			NFIDENTID ident = varObjectList.ObjectVal(i);

			NFIObject* pObject = m_pKernelModule->GetObject(ident);
			if (pObject)
			{
				list.Add(pObject->GetPropertyManager());
			}
		}

		NFMsg::MultiObjectPropertyList xData;
		PropertyListToString(list, xData, E_CHECK_TYPE::ECT_PUBLIC, true);

		//////////////////////////////////////////////////////////////////////////

		NFCValueList varLoginObjectList;
		m_pKernelModule->GetContainerOnLineList(mnLoginContainerID, varLoginObjectList);
		if (varLoginObjectList.GetCount() > 0)
		{
			for (int i = 0; i < varLoginObjectList.GetCount(); i++)
			{
				NFIDENTID ident = varLoginObjectList.ObjectVal(i);
				int nFD = m_pKernelModule->QueryPropertyInt(ident, "FD");

				SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
			}
		}
	}
}