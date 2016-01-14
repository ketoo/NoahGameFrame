// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCLoginToMasterModule.h"
#include "NFLoginNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFCLoginToMasterModule::Init()
{
	return true;
}

bool NFCLoginToMasterModule::Shut()
{
	return true;
}

bool NFCLoginToMasterModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pLoginLogicModule = pPluginManager->FindModule<NFILoginLogicModule>("NFCLoginLogicModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>("NFCLoginNet_ServerModule");

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLoginLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLoginNet_ServerModule);

    m_pEventProcessModule->AddEventCallBack(NFGUID(), NFED_ON_CLIENT_SELECT_SERVER, this, &NFCLoginToMasterModule::OnSelectServerEvent);

	NFIClusterClientModule::Bind(this, &NFCLoginToMasterModule::OnReciveMSPack, &NFCLoginToMasterModule::OnSocketMSEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER )
            {
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPE)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strName;

				NFIClusterClientModule::AddServer(xServerData);
			}
		}
	}



    return true;
}

bool NFCLoginToMasterModule::BeforeShut()
{

    return false;
}

int NFCLoginToMasterModule::OnSelectServerEvent(const NFGUID& object, const int nEventID, const NFIDataList& var)
{
	if (4 != var.GetCount()
		|| !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_OBJECT,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
	{
		return -1;
	}

	const int nServerID = var.Int(0);
	const NFGUID xClientIdent = var.Object(1);
	const int nLoginID = var.Int(2);
	const std::string& strAccount = var.String(3);

	NFMsg::ReqConnectWorld xData;
	xData.set_world_id(nServerID);
	xData.set_login_id(nLoginID);
	xData.mutable_sender()->CopyFrom(NFINetModule::NFToPB(xClientIdent));
	xData.set_account(strAccount);

	SendSuitByPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData);

	return 0;
}

bool NFCLoginToMasterModule::Execute()
{
	return NFIClusterClientModule::Execute();
}

void NFCLoginToMasterModule::Register(NFINet* pNet)
{
	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_LOGIN && pPluginManager->AppID() == nServerID)
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

				NF_SHARE_PTR<ConnectData> pServerData = GetServerNetInfo(pNet);
				if (pServerData)
				{
					int nTargetID = pServerData->nGameID;
					SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED, xMsg);

					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

int NFCLoginToMasterModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;	
    NFMsg::AckConnectWorldResult xMsg;
    if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return 0;
    }

    NFCDataList var;
    var << xMsg.world_id()
        << NFINetModule::PBToNF(xMsg.sender())
        << xMsg.login_id()
        << xMsg.account()
        << xMsg.world_ip()
        << xMsg.world_port()
        << xMsg.world_key();

    m_pEventProcessModule->DoEvent(NFGUID(), NFED_ON_CLIENT_SELECT_SERVER_RESULTS, var);

	return 0;
}

void NFCLoginToMasterModule::OnReciveMSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
    //统一解包
    switch (nMsgID)
    {
    case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
        OnSelectServerResultProcess(nSockIndex, nMsgID, msg, nLen);
        break;
    case NFMsg::EGameMsgID::EGMI_STS_NET_INFO:
        OnWorldInfoProcess(nSockIndex, nMsgID, msg, nLen);
        break;
    default:
        printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
        break;
    }
}

void NFCLoginToMasterModule::OnSocketMSEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register(pNet);
    }
}

int NFCLoginToMasterModule::OnWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID ;	
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

	int nSize = xMsg.server_list_size();
	for (int i = 0; i < nSize; ++i)
	{
		const NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        NF_SHARE_PTR<NFMsg::ServerInfoReport> pServerData = mWorldMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
            *pServerData = *pData;

            mWorldMap.AddElement(pData->server_id(), pServerData);
        }
      
	}

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSize), "", "WorldInfo");

	return 0;
}

NFMapEx<int, NFMsg::ServerInfoReport>& NFCLoginToMasterModule::GetWorldMap()
{
    return mWorldMap;
}