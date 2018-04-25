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
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFNetPlugin/NFIHttpClient.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpClientModule
        : public NFIHttpClientModule
{
public:
    NFCHttpClientModule(NFIPluginManager* p);

    virtual ~NFCHttpClientModule();

	virtual bool Init();

	virtual bool AfterInit();

    virtual bool Execute();

    virtual bool Shut();

	//actually, sync post method
	//NFWebStatus
	virtual int Post(const std::string& strUri, const std::string& strData, std::string& strResData);
	virtual int Post(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, const std::string& strData, std::string& strResData);

	//actually, sync get method
	//NFWebStatus
	virtual int Get(const std::string& strUri, std::string& strResData);
	virtual int Get(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, std::string& strResData);
protected:
    virtual bool DoGet(const std::string& strUri,
                            const std::map<std::string, std::string>& xHeaders,
                            HTTP_RESP_FUNCTOR_PTR pCB);

    virtual bool DoPost(const std::string& strUri,
                             const std::map<std::string, std::string>& xHeaders,
                             const std::string& strPostData,
                             HTTP_RESP_FUNCTOR_PTR pCB);


protected:

	class RespData
	{
	public:
		RespData()
		{
			resp = false;
			time = 0;
			state_code = 0;
		}

		bool resp;
		int time;
		int state_code;
		std::string strRespData;
	};

	void CallBack(const NFGUID id, const int state_code, const std::string& strRespData);

private:
	NFIKernelModule* m_pKernelModule;
	NFIHttpClient* m_pHttpClient;
    std::map<std::string, std::string> m_xDefaultHttpHeaders;
	NFMapEx<NFGUID, RespData> mxRespDataMap;
};

#endif
