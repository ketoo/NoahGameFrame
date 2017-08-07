// -------------------------------------------------------------------------
//    @FileName			:    NFCNetLogic.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCNetLogic
//
// -------------------------------------------------------------------------

#include "stdafx.h"
#include "NFCNetLogic.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCNetLogic::Init()
{
    return true;
}

bool NFCNetLogic::Shut()
{
    return true;
}

bool NFCNetLogic::ReadyExecute()
{
    return true;
}

bool NFCNetLogic::Execute()
{


    return true;
}

bool NFCNetLogic::AfterInit()
{
	NFCLogicBase::AfterInit();
	g_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_NONE, this, &NFCNetLogic::OnSocketEvent);
	g_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_NONE, this, &NFCNetLogic::OnMsgRecive);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_EVENT_RESULT, this, &NFCNetLogic::OnEventResult);

    return true;
}

void NFCNetLogic::ConnectServer(const std::string strIp, int nPort)
{
	m_listDelayMsg.clear();
	m_bSocketReady = false;

	NF_SHARE_PTR<ConnectData> serverData = g_pNetClientModule->GetServerNetInfo(0);
	if (serverData)
	{
		serverData->eState = ConnectDataState::RECONNECT;
		// server config error, fix it
		if (strIp != "127.0.0.1")
		{
			serverData->strIP = strIp;
		}
		serverData->nPort = nPort; 
		serverData->mnLastActionTime = GetPluginManager()->GetNowTime()-9;
	}
	else
	{
		ConnectData xServerData;
		xServerData.nGameID = 0;
		xServerData.strIP = strIp;
		xServerData.nPort = nPort;
		g_pNetClientModule->AddServer(xServerData);
	}
}

void NFCNetLogic::SendToServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData)
{
	if(m_bSocketReady)
	{
		g_pNetClientModule->SendToAllServerByPB(nMsgID, xData);
	}
	else
	{
        NFMsg::MsgBase xMsg;
        xData.SerializeToString(xMsg.mutable_msg_data());
        NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
        *pPlayerID = NFINetModule::NFToPB(NFGUID());

        std::string strMsg;
        xMsg.SerializeToString(&strMsg);
		m_listDelayMsg.push_back(std::make_pair((uint16_t)nMsgID, strMsg));
	}
}

NFINetClientModule *NFCNetLogic::GetNetModule()
{	
	if(!g_pNetClientModule)
		g_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return g_pNetClientModule;
}

void NFCNetLogic::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		m_bSocketReady = true;

		while(m_listDelayMsg.size() > 0)
		{
			auto msg = m_listDelayMsg.front();

			g_pNetClientModule->SendToAllServer(msg.first, msg.second);

			m_listDelayMsg.pop_front();
		}
    }
}

void NFCNetLogic::OnEventResult(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckEventResult xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
}

void NFCNetLogic::OnMsgRecive(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CCLOG("MsgRecv:%d not Register", nMsgID);
}
