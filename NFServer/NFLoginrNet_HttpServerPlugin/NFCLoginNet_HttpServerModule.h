// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_HttpServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-01
//    @Module           :    NFCLoginNet_HttpServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LOGIN_NET_HTTP_MODULE_H
#define NFC_LOGIN_NET_HTTP_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
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

	bool OnCommonQuery(const NFHttpRequest& req);

	NFWebStatus OnFilter(const NFHttpRequest& req);

private:
	NFIKernelModule* m_pKernelModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFILoginNet_ServerModule* m_pLoginServerModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
};

#endif