#ifndef NFC_HTTPNET_MODULE_H
#define NFC_HTTPNET_MODULE_H

#include <iostream>
#include "NFIHttpNetModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpNetModule : public NFIHttpNetModule
{
public:
	NFCHttpNetModule(NFIPluginManager* p);
	~NFCHttpNetModule();

	virtual bool Init();
	virtual bool Execute();
public:
	virtual int InitServer(const unsigned short nPort);
	virtual bool AddMsgCB(const int msgId, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);

	virtual NFIHttpNet* GetHttpNet();
	void OnReceiveNetPack(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap);

private:
	NFILogModule* mLogModule;

	NFIHttpNet* mHttpNet;
	std::map<int, HTTPNET_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
	std::list<HTTPNET_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};
#endif
