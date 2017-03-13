#include "stdwx.h"
#include "NetWorkThread.h"
#include "NFCNet.h"

NetWorkThread::NetWorkThread(wxDialog* parent, const std::string& strIP, int nPort, unsigned int nThreadID)
{
	m_parent = parent;
	m_bStop = false;
	m_pNet = nullptr;

	m_strIP = strIP;
	m_nPort = nPort;
	m_nThreadID = nThreadID;

	if (m_strIP.length() > 0 && m_nPort != 0)
	{
		if (m_pNet == nullptr)
		{
			m_pNet = new NFCNet(this, &NetWorkThread::OnRecvMsgHandler, &NetWorkThread::OnSockEventHandler);
			m_pNet->Initialization(m_strIP.c_str(), m_nPort);
		}
	}
}

void NetWorkThread::OnRecvMsgHandler(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, ID_RECV_PROTO_MSG);
	event.SetInt(nMsgID);
	m_parent->GetEventHandler()->AddPendingEvent(event);
}

void NetWorkThread::OnSockEventHandler(const int nSockIndex, const NF_NET_EVENT e, NFINet* p)
{
	wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, ID_RECV_CONNECT_MSG);

	if (e == NF_NET_EVENT_CONNECTED)
	{		
		event.SetString("connect server success.");
	}
	else 
	{
		event.SetString("lost connection.");
	}
	m_parent->GetEventHandler()->AddPendingEvent(event);
}

void NetWorkThread::SendMsg(uint16_t nMsgID, const std::string& msgData)
{
	{
		wxMutexLocker locker(m_Mutex);
		NetMsg msg;
		msg.nMsgID = nMsgID;
		msg.strMsgData = msgData;
		m_MsgList.push(msg);
	}
}

wxThread::ExitCode NetWorkThread::Entry()
{
	while (!m_bStop)
	{
		if (TestDestroy())
		{
			break;
		}

		if (m_pNet)
		{
			this->Sleep(200);
			{
				wxMutexLocker locker(m_Mutex);
				while (m_MsgList.size())
				{
					NetMsg msg = m_MsgList.front();
					m_pNet->SendMsgWithOutHead(msg.nMsgID, msg.strMsgData.c_str(), msg.strMsgData.length(), 0);
					m_MsgList.pop();
				}
			}
			m_pNet->Execute();
		}
	}
	return 0;
}

void NetWorkThread::OnExit()
{
	if (m_pNet)
	{
		delete m_pNet;
		m_pNet = nullptr;
	}
}
