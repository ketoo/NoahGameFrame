#ifndef NFC_MASTERNET_HTTP_MODULE_H
#define NFC_MASTERNET_HTTP_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_HttpServerModule.h"

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
	bool OnCommandQuery(const NFHttpRequest& req);

	bool OnCommonQuery(const NFHttpRequest& req);

private:
	NFIKernelModule* m_pKernelModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFIMasterNet_ServerModule* m_pMasterServerModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;

	std::string m_strWebRootPath;
};

#endif