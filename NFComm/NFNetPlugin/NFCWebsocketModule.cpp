// -------------------------------------------------------------------------
//    @FileName			:    NFCWebsocketModule.cpp
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    NFCWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCWebsocketModule.h"

NFCWebsocketModule::NFCWebsocketModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCWebsocketModule::Init()
{
	m_pWSServer = new NFCWS(this, &NFCWebsocketModule::OnWebsocketMessage, &NFCWebsocketModule::OnWebsocketEvent);

	return true;
}

bool NFCWebsocketModule::AfterInit()
{
	return true;
}

bool NFCWebsocketModule::Shut()
{
	return true;
}

bool NFCWebsocketModule::BeforeShut()
{
	return true;
}

bool NFCWebsocketModule::Execute()
{
	m_pWSServer->Execute();
	return true;
}

void NFCWebsocketModule::SetReceiveCallBack(NF_WS_MSG_CALL_BACK_PTR functorPtr)
{
	m_pRecvMsgCB = functorPtr;
}

void NFCWebsocketModule::SetEventCallBack(NF_WS_EVENT_CALL_BACK_PTR functorPtr)
{
	m_pEvtCB = functorPtr;
}

int NFCWebsocketModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount /*= 4*/)
{
	return m_pWSServer->Initialization(nMaxClient,nPort,nCpuCount);
}

bool NFCWebsocketModule::SendMsgToAllClient(const char* msg, const uint32_t nLen, NF_WS_MSG_DATA_TYPE msg_data_type /*= TEXT*/)
{
	return m_pWSServer->SendMsgToAllClient(msg,nLen,msg_data_type);
}

bool NFCWebsocketModule::SendMsgToClient(const char* msg, const uint32_t nLen, const std::vector<websocketpp::connection_hdl>& conn_list, NF_WS_MSG_DATA_TYPE msg_data_type /*= TEXT*/)
{
	return m_pWSServer->SendMsgToClient(msg, nLen,conn_list,msg_data_type);
}

bool NFCWebsocketModule::SendMsgToClient(const char* msg, const uint32_t nLen, websocketpp::connection_hdl conn, NF_WS_MSG_DATA_TYPE msg_data_type /*= TEXT*/)
{
	return m_pWSServer->SendMsgToClient(msg, nLen, conn, msg_data_type);
}

void NFCWebsocketModule::OnWebsocketMessage(websocketpp::connection_hdl hd, const std::string & strPayload,NF_WS_MSG_DATA_TYPE msg_data_type)
{
	if (m_pRecvMsgCB.get())
	{
		m_pRecvMsgCB->operator()(hd, strPayload,msg_data_type);
	}
}

void NFCWebsocketModule::OnWebsocketEvent(websocketpp::connection_hdl hd, NF_WS_EVENT evt)
{
	if (m_pEvtCB.get())
	{
		m_pEvtCB->operator()(hd, evt);
	}
}