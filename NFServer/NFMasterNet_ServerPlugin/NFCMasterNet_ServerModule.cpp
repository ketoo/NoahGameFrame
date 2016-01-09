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
	return true;
}

bool NFCMasterNet_ServerModule::Shut()
{
	return true;
}

int NFCMasterNet_ServerModule::OnWorldRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mWorldMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "WorldRegistered");
    }


	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        mWorldMap.RemoveElement(pData->server_id());


        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "WorldUnRegistered");
    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mWorldMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "RefreshWorldInfo");

    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnLoginRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mLoginMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "LoginRegistered");
    }

	SynWorldToLogin();

	return 0;
}

int NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        mLoginMap.RemoveElement(pData->server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "LoginUnRegistered");

    }


	return 0;
}

int NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mLoginMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "RefreshLoginInfo");

    }

	return 0;
}

int NFCMasterNet_ServerModule::OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqConnectWorld xMsg;
	if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(xMsg.world_id());
    if (!pServerData.get())
    {
        return 0;
    }

	//ת���͵����������
	SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg, pServerData->nFD);

	return 0;
}

bool NFCMasterNet_ServerModule::Execute()
{
	LogGameServer();

	NFINetModule::Execute();
	return true;
}

int NFCMasterNet_ServerModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckConnectWorldResult xMsg;
	if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

    NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(xMsg.login_id());
    if (!pServerData.get())
    {
        return 0;
    }

	//ת���͵���¼������
	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, pServerData->nFD);

	return 0;
}

bool NFCMasterNet_ServerModule::AfterInit()
{
	m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER && pPluginManager->AppID() == nServerID)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				Initialization(this, &NFCMasterNet_ServerModule::OnRecivePack, &NFCMasterNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);
			}
		}
	}

	return true;
}

void NFCMasterNet_ServerModule::OnRecivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	switch (nMsgID)
	{
    case NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT:
        break;
	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_REGISTERED:
		OnWorldRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_UNREGISTERED:
		OnWorldUnRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_MTL_WORLD_REFRESH:
		OnRefreshWorldInfoProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED:
		OnLoginRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_UNREGISTERED:
		OnLoginUnRegisteredProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REFRESH:
		OnRefreshLoginInfoProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD:
		OnSelectWorldProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
		OnSelectServerResultProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	default:
		printf("NFNet || �Ƿ���Ϣ:unMsgID=%d\n", nMsgID);
		break;
	}
}

void NFCMasterNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
{
	//std::cout << "OnSocketEvent::thread id=" << GetCurrentThreadId() << std::endl;

    if (eEvent & NF_NET_EVENT_EOF)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }
}

void NFCMasterNet_ServerModule::OnClientDisconnect( const int nAddress )
{
	//������login����world��Ҫ�ҳ���,������ע��
    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.First();
    while (pServerData.get())
    {
        if (nAddress == pServerData->nFD)
        {
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

            SynWorldToLogin();
            return;
        }

        pServerData = mWorldMap.Next();
    }

    //////////////////////////////////////////////////////////////////////////

    int nServerID = 0;
    pServerData =  mLoginMap.First();
    while (pServerData.get())
    {
        if (nAddress == pServerData->nFD)
        {
            nServerID = pServerData->pData->server_id();
            break;
        }

        pServerData = mLoginMap.Next();
    }

    mLoginMap.RemoveElement(nServerID);

}

void NFCMasterNet_ServerModule::OnClientConnected( const int nAddress )
{
	//��������ɶ������
}

void NFCMasterNet_ServerModule::SynWorldToLogin()
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.First();
    while (pServerData.get())
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mWorldMap.Next();
    }

    //�㲥������loginserver
    pServerData =  mLoginMap.First();
    while (pServerData.get())
    {
        SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, pServerData->nFD);

        pServerData = mLoginMap.Next();
    }
}

void NFCMasterNet_ServerModule::LogGameServer()
{
	if (mnLastLogTime + 10 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	mnLastLogTime = GetPluginManager()->GetNowTime();

	//////////////////////////////////////////////////////////////////////////

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log WorldServer Info", "");

	NF_SHARE_PTR<ServerData> pGameData = mWorldMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

		pGameData = mWorldMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log WorldServer Info", "");

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log LoginServer Info", "");

	//////////////////////////////////////////////////////////////////////////
	pGameData = mLoginMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

		pGameData = mLoginMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log LoginServer Info", "");

}