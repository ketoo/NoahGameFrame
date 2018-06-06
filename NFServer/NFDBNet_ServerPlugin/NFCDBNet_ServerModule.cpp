// -------------------------------------------------------------------------
//    @FileName         :    NFCDBNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDBNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCDBNet_ServerModule.h"
#include "NFDBNet_ServerPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCDBNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

    return true;
}

bool NFCDBNet_ServerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_REGISTERED, this, &NFCDBNet_ServerModule::OnGameServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_GTW_GAME_UNREGISTERED, this, &NFCDBNet_ServerModule::OnGameServerUnRegisteredProcess);

    m_pNetModule->AddEventCallBack(this, &NFCDBNet_ServerModule::OnSocketEvent);
	m_pNetModule->ExpandBufferSize();

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && pPluginManager->GetAppID() == nServerID)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                //const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
                //const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
                if (nRet < 0)
                {
                    std::ostringstream strLog;
                    strLog << "Cannot init server net, Port = " << nPort;
                    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
                    NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
                    exit(0);
                }
            }
        }
    }

    return true;
}

/*
void NFCDBNet_ServerModule::OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		if (xData.server_type() == NF_SERVER_TYPES::NF_ST_WORLD)
		{
			NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
				mWorldMap.AddElement(xData.server_id(), pServerData);
			}

			pServerData->nFD = nSockIndex;
			*(pServerData->pData) = xData;

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
		}
	}

	//sync to proxy

	//sync to game

}
*/
bool NFCDBNet_ServerModule::Shut()
{

    return true;
}

bool NFCDBNet_ServerModule::Execute()
{

	return true;
}

void NFCDBNet_ServerModule::OnGameServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(xData.server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

}

void NFCDBNet_ServerModule::OnGameServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        mGameMap.RemoveElement(xData.server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }
}

void NFCDBNet_ServerModule::OnRefreshWorldServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(xData.server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "GameServerRegistered");
    }

}

void NFCDBNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
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

void NFCDBNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
}

void NFCDBNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{
}

