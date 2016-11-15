#ifndef NFC_MASTERNET_WEB_MODULE_H
#define NFC_MASTERNET_WEB_MODULE_H

#include "NFComm/NFPluginModule/NFIMasterNet_WebServerModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
extern "C" {
#include "Dependencies/miniweb/httppil.h"
#include "Dependencies/miniweb/httpapi.h"
#include "Dependencies/miniweb/revision.h"
#include "Dependencies/miniweb/httpint.h"
}
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

class NFCMasterNet_WebServerModule
	: public NFIMasterNet_WebServerModule
{
public:
	NFCMasterNet_WebServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute();

	virtual bool BeforeShut();

protected:
	char* GetLocalAddrString();;
	void GetFullPath(char* buffer, const char* path);

private:
	NFIKernelModule* mKernelModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;

	HttpParam httpParam;
	bool isInitSuccess = false;

	HttpParam *hp = &httpParam;
	HttpSocket *phsSocketCur;
	SOCKET socket;
	struct sockaddr_in sinaddr;

	int iRc;
	int i;

	int nWebPort;
	std::string strWebRootPath;
};

#endif