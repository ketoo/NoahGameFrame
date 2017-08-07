#include "NFCNetModule.h"
// -------------------------------------------------------------------------
//    @FileName         :    NFCNetModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFCNetModule
//
// -------------------------------------------------------------------------

NFCNetModule::NFCNetModule(NFIPluginManager * p)
{
	pPluginManager = p;

	mnBufferSize = 0;
	nLastTime = GetPluginManager()->GetNowTime();
	m_pNet = NULL;
}

NFCNetModule::~NFCNetModule()
{
	if (m_pNet)
	{
		m_pNet->Final();
	}

	delete m_pNet;
	m_pNet = NULL;
}
void NFCNetModule::Initialization(const char * strIP, const unsigned short nPort)
{
	m_pNet = NF_NEW NFCNet(this, &NFCNetModule::OnReceiveNetPack, &NFCNetModule::OnSocketNetEvent);
	m_pNet->ExpandBufferSize(mnBufferSize);
	m_pNet->Initialization(strIP, nPort);
}

int NFCNetModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
	m_pNet = NF_NEW NFCNet(this, &NFCNetModule::OnReceiveNetPack, &NFCNetModule::OnSocketNetEvent);
	m_pNet->ExpandBufferSize(mnBufferSize);
	return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
}

int NFCNetModule::ExpandBufferSize(const unsigned int size)
{
	if (size > 0)
	{
		mnBufferSize = size;
		if (m_pNet)
		{
			m_pNet->ExpandBufferSize(mnBufferSize);
		}
	}

	return mnBufferSize;
}

void NFCNetModule::RemoveReceiveCallBack(const int nMsgID)
{
	std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
	if (mxReceiveCallBack.end() != it)
	{
		mxReceiveCallBack.erase(it);
	}
}

bool NFCNetModule::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR & cb)
{
	if (mxReceiveCallBack.find(nMsgID) != mxReceiveCallBack.end())
	{
		return false;
	}

	mxReceiveCallBack.insert(std::map<int, NET_RECEIVE_FUNCTOR_PTR>::value_type(nMsgID, cb));

	return true;
}

bool NFCNetModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR & cb)
{
	mxCallBackList.push_back(cb);

	return true;
}

bool NFCNetModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR & cb)
{
	mxEventCallBackList.push_back(cb);

	return true;
}

bool NFCNetModule::Execute()
{
	if (!m_pNet)
	{
		return false;
	}


	KeepAlive();

	return m_pNet->Execute();
}

bool NFCNetModule::SendMsgWithOutHead(const int nMsgID, const std::string & msg, const NFSOCK nSockIndex)
{
	return m_pNet->SendMsgWithOutHead(nMsgID, msg.c_str(), (uint32_t) msg.length(), nSockIndex);
}

bool NFCNetModule::SendMsgToAllClientWithOutHead(const int nMsgID, const std::string & msg)
{
	return m_pNet->SendMsgToAllClientWithOutHead(nMsgID, msg.c_str(), (uint32_t) msg.length());
}

bool NFCNetModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message & xData, const NFSOCK nSockIndex)
{
	NFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", (int)nSockIndex, nMsgID);

		return false;
	}

	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(NFGUID());

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", (int)nSockIndex, nMsgID);

		return false;
	}

	SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);

	return true;
}

bool NFCNetModule::SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message & xData)
{
	NFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to all Failed For Serialize of MsgData, MessageID: %d\n", nMsgID);

		return false;
	}

	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(NFGUID());

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to all Failed For Serialize of MsgBase, MessageID: %d\n", nMsgID);

		return false;
	}

	return SendMsgToAllClientWithOutHead(nMsgID, strMsg);
}

bool NFCNetModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message & xData, const NFSOCK nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList)
{
	if (!m_pNet)
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", (int)nSockIndex, nMsgID);

		return false;
	}

	NFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", (int)nSockIndex, nMsgID);

		return false;
	}


	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(nPlayer);
	if (pClientIDList)
	{
		for (int i = 0; i < pClientIDList->size(); ++i)
		{
			const NFGUID& ClientID = (*pClientIDList)[i];

			NFMsg::Ident* pData = xMsg.add_player_client_list();
			if (pData)
			{
				*pData = NFToPB(ClientID);
			}
		}
	}

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", (int)nSockIndex, nMsgID);

		return false;
	}

	return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
}

bool NFCNetModule::SendMsgPB(const uint16_t nMsgID, const std::string & strData, const NFSOCK nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList)
{
	if (!m_pNet)
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", (int)nSockIndex, nMsgID);
		//LogSend(szData);

		return false;
	}

	NFMsg::MsgBase xMsg;
	xMsg.set_msg_data(strData.data(), strData.length());


	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(nPlayer);
	if (pClientIDList)
	{
		for (int i = 0; i < pClientIDList->size(); ++i)
		{
			const NFGUID& ClientID = (*pClientIDList)[i];

			NFMsg::Ident* pData = xMsg.add_player_client_list();
			if (pData)
			{
				*pData = NFToPB(ClientID);
			}
		}
	}

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", (int)nSockIndex, nMsgID);
		//LogSend(szData);

		return false;
	}

	return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
}

NFINet * NFCNetModule::GetNet()
{
	return m_pNet;
}

void NFCNetModule::OnReceiveNetPack(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
	if (mxReceiveCallBack.end() != it)
	{
		NET_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
	}
	else
	{
		for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxCallBackList.begin(); it != mxCallBackList.end(); ++it)
		{
			NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
			NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
		}
	}
}
void NFCNetModule::OnSocketNetEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet * pNet)
{
	for (std::list<NET_EVENT_FUNCTOR_PTR>::iterator it = mxEventCallBackList.begin(); it != mxEventCallBackList.end(); ++it)
	{
		NET_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(nSockIndex, eEvent, pNet);
	}
}

void NFCNetModule::KeepAlive()
{
	if (!m_pNet)
	{
		return;
	}

	if (m_pNet->IsServer())
	{
		return;
	}

	if (nLastTime + 10 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	nLastTime = GetPluginManager()->GetNowTime();

	NFMsg::ServerHeartBeat xMsg;
	xMsg.set_count(0);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, 0);

}