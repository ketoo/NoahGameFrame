////////////////////////////////////////////
//  Filename NetWorkThread.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/03/13
//
////////////////////////////////////////////

#ifndef PROTOCOL_THREAD_H__
#define PROTOCOL_THREAD_H__

#include <atomic>
#include <wx/thread.h>
#include <queue>
#include "NFComm/NFNetPlugin/NFINet.h"

// a thread class that will periodically send events to the GUI thread
class NetWorkThread : public wxThread
{
	wxDialog* m_parent;
	std::atomic_bool m_bStop;

	NFINet*	m_pNet;

	std::string m_strIP;
	unsigned short      m_nPort;

	wxMutex		m_Mutex;
	std::queue<NetMsg>	m_MsgList;
	unsigned int m_nThreadID;

public:
	NetWorkThread(wxDialog* parent, const std::string& strIP, int nPort, unsigned int nThreadID=0);

	void OnRecvMsgHandler(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnSockEventHandler(const int nSockIndex, const NF_NET_EVENT e, NFINet* p);

	void SendMsg(uint16_t nMsgID,const std::string& msgData );


	void Stop(){
		m_bStop = true;
	}

	virtual ExitCode Entry();
	virtual void OnExit();
};


#endif // PROTOCOL_THREAD_H__
