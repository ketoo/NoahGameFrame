#ifndef NFC_HTTPNET_MODULE_H
#define NFC_HTTPNET_MODULE_H

#include <iostream>
#include "NFComm/NFNetPlugin/NFCHttpNet.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpServerModule
	: public NFIHttpServerModule
{

public:
	NFCHttpServerModule(NFIPluginManager* p);
	virtual ~NFCHttpServerModule();
	
public:
	virtual bool Execute();

	virtual int InitServer(const unsigned short nPort);
	
	void OnReceiveNetPack(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl);
	
	virtual bool AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);
	
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);
	
	virtual NFIHttpNet* GetHttpNet();

private:
	NFILogModule* mLogModule;

	NFCHttpNet* mHttpNet;
	std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
	std::list<HTTPNET_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};

#endif
