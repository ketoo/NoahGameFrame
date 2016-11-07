#ifndef NFC_MASTERNET_HTTP_MODULE_H
#define NFC_MASTERNET_HTTP_MODULE_H

#include "NFIMasterNet_HttpServerModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFNet/NFCHttpNetModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

class NFCMasterNet_HttpServerModule
	: public NFIMasterNet_HttpServerModule
{
public:
	NFCMasterNet_HttpServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute();

protected:
	void OnQueryServerStatus(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap);
	void InvalidMessage(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap);

private:
	NFIKernelModule* mKernelModule;
	NFIHttpNetModule* mHttpNetModule;

	NFIMasterNet_ServerModule* mMasterServerModule;
};

#endif