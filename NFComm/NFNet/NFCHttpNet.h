#ifndef NFC_HTTP_NET_H
#define NFC_HTTP_NET_H

#include "NFIHttpNet.h"
#include "NFINet.h"

class NFCHttpNet : public NFIHttpNet
{
public:
	template<typename BaseType>
	NFCHttpNet(BaseType* pBaseType, void (BaseType::*handleRecieve)(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap))
	{
		base = NULL;
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		mPort = 0;
	}
	NFCHttpNet()
	{

	}
	virtual ~NFCHttpNet() {};

	virtual bool Execute();
public:
	virtual int InitServer(const unsigned short nPort);
	static void listener_cb(struct evhttp_request *req, void *arg);
	static bool SendMsg(struct evhttp_request *req, const std::string& strMsg);
	virtual bool Final();
private:
	static std::vector<std::string> Split(const std::string& str, std::string delim);
private:
	int mPort;
	struct event_base* base;

	HTTPNET_RECEIVE_FUNCTOR mRecvCB;

};

#endif
