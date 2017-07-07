#ifndef NFC_MASTERNET_HTTP_MODULE_H
#define NFC_MASTERNET_HTTP_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpNetModule.h"
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
	void OnCommandQuery(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl);

	void OnCommonQuery(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl);

private:
	NFIKernelModule* m_pKernelModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFIMasterNet_ServerModule* m_pMasterServerModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;

	std::string m_strWebRootPath;
};

#endif