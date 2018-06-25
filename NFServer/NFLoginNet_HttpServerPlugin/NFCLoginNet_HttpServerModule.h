// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_HttpServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-01
//    @Module           :    NFCLoginNet_HttpServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LOGIN_NET_HTTP_MODULE_H
#define NFC_LOGIN_NET_HTTP_MODULE_H

#include <map>
#include <iostream>

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILoginToMasterModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_HttpServerModule.h"

class NFCLoginNet_HttpServerModule
	: public NFILoginNet_HttpServerModule
{
public:
	NFCLoginNet_HttpServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute();

protected:
	bool OnLogin(const NFHttpRequest& req);
	bool OnWorldView(const NFHttpRequest& req);
	bool OnWorldSelect(const NFHttpRequest& req);

	bool OnCommonQuery(const NFHttpRequest& req);

	NFWebStatus OnFilter(const NFHttpRequest& req);

	std::string GetUserID(const NFHttpRequest & req);
	std::string GetUserJWT(const NFHttpRequest & req);
	bool CheckUserJWT(const std::string & user, const std::string & jwt);


	std::map<std::string, std::string> mToken;

private:
	NFINetClientModule* m_pNetClientModule;
	NFIKernelModule* m_pKernelModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFILoginNet_ServerModule* m_pLoginServerModule;
	NFILoginToMasterModule* m_pLoginToMasterModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
};

#endif