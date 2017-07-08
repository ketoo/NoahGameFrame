// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpClientModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-07-07
//    @Module           :   NFCHttpClientModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HTTP_CLIENT_MODULE_H
#define NFC_HTTP_CLIENT_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpClientModule
	: public NFIHttpClientModule
{

public:
	NFCHttpClientModule(NFIPluginManager* p);
	virtual ~NFCHttpClientModule();
	
	virtual bool Init();

	virtual bool Execute();
	
	virtual bool Shut();

	virtual bool PerformGet(const std::string& strUri, const std::string& strUserData, HTTP_RESP_FUNCTOR_PTR pCB);

	virtual bool PerformPost(const std::string& strUri, const std::string& strUserData, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB);


private:
	NFIHttpClient* m_pHttpClient;
};

#endif
