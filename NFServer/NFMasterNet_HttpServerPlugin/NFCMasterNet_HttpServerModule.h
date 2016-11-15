#ifndef NFC_MASTERNET_HTTP_MODULE_H
#define NFC_MASTERNET_HTTP_MODULE_H

#include "NFComm/NFPluginModule/NFIMasterNet_JsonServerModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"


class NFCMasterNet_HttpJsonModule
	: public NFIMasterNet_JsonServerModule
{
public:
	NFCMasterNet_HttpJsonModule(NFIPluginManager* p)
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
	NFIHttpServerModule* mHttpNetModule;

	NFIMasterNet_ServerModule* mMasterServerModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
};

#endif