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
        ServerData* pServerData =  mWorldMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mWorldMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

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
        ServerData* pServerData =  mWorldMap.RemoveElement(pData->server_id());
        if (pServerData)
        {
            delete pServerData;
            pServerData = NULL;
        }

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
        ServerData* pServerData =  mWorldMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mWorldMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

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
        ServerData* pServerData =  mLoginMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mLoginMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

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
        ServerData* pServerData =  mLoginMap.RemoveElement(pData->server_id());
        if (pServerData)
        {
            delete pServerData;
            pServerData = NULL;
        }

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
        ServerData* pServerData =  mLoginMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mLoginMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

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

    ServerData* pServerData =  mWorldMap.GetElement(xMsg.world_id());
    if (!pServerData)
    {
        return 0;
    }

	//转发送到世界服务器
	SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg, pServerData->nFD);

	return 0;
}

bool NFCMasterNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	m_pNet->Execute(fLasFrametime, fStartedTime);

	return true;
}

int NFCMasterNet_ServerModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
	int64_t nPlayerID = 0;
	NFMsg::AckConnectWorldResult xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return 0;
	}

    ServerData* pServerData =  mLoginMap.GetElement(xMsg.login_id());
    if (!pServerData)
    {
        return 0;
    }

	//转发送到登录服务器
	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, pServerData->nFD);

	return 0;
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

	const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
	const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
	const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
	const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCMasterNet_ServerModule::OnRecivePack, &NFCMasterNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}

	return true;
}

int NFCMasterNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
	int nMsgID = msg.GetMsgHead()->GetMsgID();
	switch (nMsgID)
	{
	case NFMsg::EGameMsgID::EGMI_UNKNOW:
		break;
    case NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT:
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
    ServerData* pServerData =  mWorldMap.First();
    while (pServerData)
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
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            nServerID = pServerData->pData->server_id();
            break;
        }

        pServerData = mLoginMap.Next();
    }

    if (nServerID != 0)
    {
        ServerData* pRmServerData = mLoginMap.RemoveElement(nServerID);
        if (pRmServerData)
        {
            delete pRmServerData;
            pRmServerData = NULL;
        }
    }
}

void NFCMasterNet_ServerModule::OnClientConnected( const int nAddress )
{
	//连接上来啥都不做
}

void NFCMasterNet_ServerModule::SynWorldToLogin()
{
    NFMsg::ServerInfoReportList xData;

    ServerData* pServerData =  mWorldMap.First();
    while (pServerData)
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mWorldMap.Next();
    }

    //广播给所有loginserver
    pServerData =  mLoginMap.First();
    while (pServerData)
    {
        SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, pServerData->nFD);

        pServerData = mLoginMap.Next();
    }
}