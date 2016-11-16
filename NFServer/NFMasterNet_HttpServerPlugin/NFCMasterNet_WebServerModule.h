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

extern "C" int uhStats(UrlHandlerParam* param);

class NFCMasterNet_WebServerModule
	: public NFIMasterNet_WebServerModule
{
public:
	NFCMasterNet_WebServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
		s_pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute();

	virtual bool BeforeShut();
	static int GetMasterJson(UrlHandlerParam* param);
protected:

	char* GetLocalAddrString();;
	void GetFullPath(char* buffer, const char* path);
	static std::vector<std::string> SplitString(const std::string& str, std::string delim);


private:
	UrlHandler urlHandlerList[3] = {
		{ "stats", uhStats, NULL },
		{ "json", GetMasterJson ,NULL},
		{ NULL },
	};

	AuthHandler authHandlerList[2] = {
		{ "stats", "user", "pass", "group=admin", "" },
		{ NULL }
	};

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

	static NFIPluginManager* s_pPluginManager;
};

#endif