#ifndef NFC_HTTP_NET_H
#define NFC_HTTP_NET_H

#include "NFIHttpNet.h"

class NFCHttpNet : public NFIHttpNet
{
public:
	NFCHttpNet()
	{
	}

	template<typename BaseType>
	NFCHttpNet(BaseType* pBaseType, void (BaseType::*handleRecieve)(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl))
	{
		base = NULL;
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		mPort = 0;
	}
	
	virtual ~NFCHttpNet() {};

public:
	virtual bool Execute();
	virtual int InitServer(const unsigned short nPort);
	virtual bool Final();

	virtual bool ResponseMsg(const NFHttpRequest & req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
	
	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName);

private:
	bool ResponseFile(const NFHttpRequest& req, const int fd, struct stat st, const std::string& strType);

	static void listener_cb(struct evhttp_request *req, void *arg);
	static std::vector<std::string> Split(const std::string& str, std::string delim);

private:
	int mPort;
	struct event_base* base;
	HTTP_RECEIVE_FUNCTOR mRecvCB;
};

#endif
