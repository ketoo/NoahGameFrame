// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameServerToWorldModule.h"
#include "NFGameServerNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClusterClientModule.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCGameServerToWorldModule::Init()
{
	m_pClusterClientModule = NF_NEW NFIClusterClientModule(pPluginManager);

	m_pClusterClientModule->Init();

    return true;
}

bool NFCGameServerToWorldModule::Shut()
{

    return true;
}


bool NFCGameServerToWorldModule::Execute()
{
	m_pClusterClientModule->Execute();

	return true;
}

void NFCGameServerToWorldModule::Register(NFINet* pNet)
{
    //成功就注册
    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->AppID() == nServerID)
            {
                const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

                NFMsg::ServerInfoReportList xMsg;
                NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

                pData->set_server_id(nServerID);
                pData->set_server_name(strName);
                pData->set_server_cur_count(0);
                pData->set_server_ip(strIP);
                pData->set_server_port(nPort);
                pData->set_server_max_online(nMaxConnect);
                pData->set_server_state(NFMsg::EST_NARMAL);
                pData->set_server_type(nServerType);

                NF_SHARE_PTR<ConnectData> pServerData = m_pClusterClientModule->GetServerNetInfo(pNet);
                if (pServerData)
                {
                    int nTargetID = pServerData->nGameID;
					m_pClusterClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_GTW_GAME_REGISTERED, xMsg);

                    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}


void NFCGameServerToWorldModule::RefreshWorldInfo()
{
    //     _tagPT_KEY_BASE_MSG baseMsg;
    //     NFMsg::ServerInfoReport xMsg;
    //
    //     xMsg.set_server_id(m_pGameLogicModule->GetGameID());
    //     xMsg.set_server_port(m_pGameServerModule->GetServerPort());
    //     xMsg.set_server_cur_count(0);
    //     xMsg.set_server_max_online(50000);
    //
    //     if (xMsg.SerializeToString(&baseMsg.strSyncInfo))
    //     {
    //         baseMsg._unMsgID = GTW_GAME_REFRESH;
    //         RakNet::BitStream oBitStream;
    //         baseMsg.EnCode(&oBitStream);
    //         SendBitStream(&oBitStream);
    //     }
}

bool NFCGameServerToWorldModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pGameServerNet_ServerModule);


	m_pClusterClientModule->AddReceiveCallBack(this, &NFCGameServerToWorldModule::TransPBToProxy);

	m_pClusterClientModule->AddEventCallBack(this, &NFCGameServerToWorldModule::OnSocketWSEvent);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCGameServerToWorldModule::OnObjectClassEvent);

    // 连接world server
    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD)
            {
                const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

                ConnectData xServerData;

                xServerData.nGameID = nServerID;
                xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
                xServerData.strIP = strIP;
                xServerData.nPort = nPort;
                xServerData.strName = strName;

                m_pClusterClientModule->AddServer(xServerData);
            }
        }
    }

    return true;
}

void NFCGameServerToWorldModule::OnSocketWSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register(pNet);

    }
}

int NFCGameServerToWorldModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == "Player")
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
            SendOffline(self);
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            SendOnline(self);
        }
    }

    return 0;
}

void NFCGameServerToWorldModule::SendOnline(const NFGUID& self)
{
    NFMsg::RoleOnlineNotify xMsg;

    const NFGUID& xGuild = m_pKernelModule->GetPropertyObject(self, "GuildID");
    *xMsg.mutable_guild() = NFINetModule::NFToPB(xGuild);

	m_pClusterClientModule->SendSuitByPB(xGuild.nData64, NFMsg::EGMI_ACK_ONLINE_NOTIFY, xMsg);

}

void NFCGameServerToWorldModule::SendOffline(const NFGUID& self)
{
    NFMsg::RoleOfflineNotify xMsg;

    const NFGUID& xGuild = m_pKernelModule->GetPropertyObject(self, "GuildID");
    *xMsg.mutable_guild() = NFINetModule::NFToPB(xGuild);

	m_pClusterClientModule->SendSuitByPB(xGuild.nData64, NFMsg::EGMI_ACK_OFFLINE_NOTIFY, xMsg);

}

void NFCGameServerToWorldModule::TransPBToProxy(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    std::string strData;
    if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, strData, nPlayerID))
    {
        return;
    }

    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, strData, nPlayerID);

    return;
}

void NFCGameServerToWorldModule::SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen)
{
    m_pClusterClientModule->SendBySuit(nHashKey, nMsgID, msg, nLen);
}

NFIClusterClientModule* NFCGameServerToWorldModule::GetClusterClientModule()
{
    return m_pClusterClientModule;
}