// -------------------------------------------------------------------------
//    @FileName			:		NFCWS.cpp
//    @Author			:		Stone.xin
//    @Date				:		2016-12-22
//    @Module			:		NFCWS
// -------------------------------------------------------------------------

#include "NFCWS.h"
#include <string.h>
#include <atomic>

bool NFCWS::Execute()
{
	if (mbWorking)
	{
		ExecuteClose();
		m_EndPoint.poll_one();
	}
	return false;
}

int NFCWS::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
	if (mbWorking)
	{
		return 1;
	}

	mnPort = nPort;
	mnMaxConnect = nMaxClient;
	mnCpuCount = nCpuCount;

	m_EndPoint.init_asio();

	m_EndPoint.set_message_handler(std::bind(
		&NFCWS::OnMessageHandler, this,
		std::placeholders::_1, std::placeholders::_2
		));

	m_EndPoint.set_open_handler(std::bind(
		&NFCWS::OnOpenHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_close_handler(std::bind(
		&NFCWS::OnCloseHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_fail_handler(std::bind(
		&NFCWS::OnFailHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_pong_handler(std::bind(
		&NFCWS::OnPongHandler, this,
		std::placeholders::_1, std::placeholders::_2));

	m_EndPoint.set_interrupt_handler(std::bind(
		&NFCWS::OnInterruptHandler, this,
		std::placeholders::_1));
	
	m_EndPoint.set_pong_timeout_handler(std::bind(
		&NFCWS::OnPongTimeOutHandler, this,
		std::placeholders::_1, std::placeholders::_2));


	m_EndPoint.listen(nPort);
	m_EndPoint.start_accept();

	mbWorking = true;

	return 0;
}

bool NFCWS::Final()
{
	CloseSocketAll();
	m_EndPoint.stop_listening();

	return true;
}

bool NFCWS::SendMsgToAllClient(const char * msg, const uint32_t nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	session_list::iterator it = mmObject.begin();
	while (it!=mmObject.end())
	{
		WSObjectPtr pWSObject = it->second;
		if (pWSObject && !pWSObject->NeedRemove())
		{
			try
			{
				m_EndPoint.send(it->first, msg, nLen, websocketpp::frame::opcode::TEXT);
			}
			catch (websocketpp::exception& e)
			{
				std::cout<<"websocket exception: "<<e.what()<< " this conn will be removed." <<std::endl;
				RemoveConnection(it->first, NF_WS_EVENT_CLOSE);
			}
		}
		it++;
	}
	
	return true;
}

bool NFCWS::SendMsgToClient(const char * msg, const uint32_t nLen, const std::vector<websocketpp::connection_hdl>& vList)
{
	for (auto vIt:vList)
	{
		auto pWSObject = GetNetObject(vIt);
		if (pWSObject && !pWSObject->NeedRemove())
		{
			try
			{
				m_EndPoint.send(vIt, msg, nLen, websocketpp::frame::opcode::TEXT);
				return true;
			}
			catch (websocketpp::exception& e)
			{
				std::cout << "websocket exception: " << e.what()<<" this conn will be removed." << std::endl;	
				RemoveConnection(vIt, NF_WS_EVENT_CLOSE);
			}
		}
	}
	return false;
}

bool NFCWS::SendMsgToClient(const char * msg, const uint32_t nLen, websocketpp::connection_hdl hd)
{
	auto pWSObject = GetNetObject(hd);
	if (pWSObject && !pWSObject->NeedRemove())
	{
		try
		{
			m_EndPoint.send(hd, msg, nLen, websocketpp::frame::opcode::TEXT);
			return true;
		}
		catch (websocketpp::exception& e)
		{
			std::cout << "websocket exception: " << e.what()<<" this conn will be removed." << std::endl;
			RemoveConnection(hd, NF_WS_EVENT_CLOSE);
		}
	}
	return false;
}

bool NFCWS::AddNetObject(websocketpp::connection_hdl hd,WSObjectPtr pWSObject)
{
	auto pObject = GetNetObject(hd);
	if (pObject)
	{
		return false;
	}
	mmObject.emplace(session_list::value_type(hd,pWSObject));
	return true;
}

WSObjectPtr NFCWS::GetNetObject(websocketpp::connection_hdl hd)
{
	session_list::iterator it = mmObject.find(hd);
	if (it == mmObject.end())
	{
		return nullptr;
	}
	return it->second;
}

void NFCWS::ExecuteClose()
{
	for(auto vIt : mvRemoveObject)
	{
		CloseObject(vIt);
	}
	mvRemoveObject.clear();
}

bool NFCWS::CloseSocketAll()
{
	session_list::iterator it = mmObject.begin();
	while(it != mmObject.end())
	{
		mvRemoveObject.push_back(it->first);
		it++;
	}

	ExecuteClose();

	mmObject.clear();

	return true;
}

void NFCWS::CloseObject(websocketpp::connection_hdl hd, int nCloseCode/* =1000 */, const std::string& strCloseReason/* ="" */)
{
	m_EndPoint.close(hd, nCloseCode, strCloseReason);
}

void NFCWS::OnMessageHandler(websocketpp::connection_hdl hd, NFWebSockConf::message_ptr msg)
{
	auto pObject = GetNetObject(hd);
	if (!pObject)
	{
		return;
	}

	if (mRecvCB)
	{
		mRecvCB(hd,msg->get_payload());
	}
}

void NFCWS::OnOpenHandler(websocketpp::connection_hdl hd)
{
	WSObjectPtr pWSObject(NF_NEW(WSObject));
	if (AddNetObject(hd,pWSObject))
	{
		if (mEventCB)
		{
			mEventCB(hd, NF_WS_EVENT_OPEN);
		}
	}
}

bool NFCWS::RemoveConnection(websocketpp::connection_hdl hd, NF_WS_EVENT evt, int nCloseCode /* = 1000 */, const std::string& strCloseReason /* = "" */)
{
	session_list::iterator it = mmObject.find(hd);
	if (it != mmObject.end())
	{
		mvRemoveObject.push_back(hd);
		return true;
	}
	return false;
}

void NFCWS::OnCloseHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, NF_WS_EVENT_CLOSE);
}

void NFCWS::OnFailHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, NF_WS_EVENT_FAIL);
}

void NFCWS::OnInterruptHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, NF_WS_EVENT_INTERRUPT);
}

bool NFCWS::OnPongHandler(websocketpp::connection_hdl hd, std::string str)
{
	return true;
}

void NFCWS::OnPongTimeOutHandler(websocketpp::connection_hdl hd, std::string str)
{
	RemoveConnection(hd, NF_WS_EVENT_PONG_TIMEOUT);
}
