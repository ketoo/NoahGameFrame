// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_URL_MODULE_H
#define NFI_URL_MODULE_H

#include <iostream>
#include "NFIModule.h"

typedef std::function<void(const NFGUID& self, const int nRet, const std::string& strRsp, const std::string& strUseData)> HTTP_RSP_FUNCTOR;
typedef std::shared_ptr<HTTP_RSP_FUNCTOR> HTTP_RSP_FUNCTOR_PTR;

class NFIUrlClientModule
    : public NFIModule
{

public:
    virtual int HttpPostRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp) = 0;
    virtual int HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp) = 0;

    template<typename BaseType>
    int HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,
                      const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, BaseType* pBaseType, void (BaseType::*handleRsp)(const NFGUID&, const int, const std::string& , const std::string&),  const std::string& strUseData)
    {
        HTTP_RSP_FUNCTOR RspCB = std::bind(handleRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return HttpRequestAs(self, strUrl, mxGetParams, strBodyData, mxCookies, fTimeOutSec, RspCB,  strUseData);
    }

    template<typename BaseType>
    int HttpRequestPostAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams,  const std::map<std::string, std::string>& mxPostParams,
                          const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, BaseType* pBaseType, void (BaseType::*handleRsp)(const NFGUID&, const int, const std::string&, const std::string&),  const std::string& strUseData)
    {
        HTTP_RSP_FUNCTOR RspCB = std::bind(handleRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return HttpRequestPostAs(self, strUrl, mxGetParams, mxPostParams, mxCookies, fTimeOutSec, RspCB,  strUseData);
    }

    virtual bool CloseActorPool() = 0;

private:
    virtual int HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn, const std::string& strUseData) = 0;
    virtual int HttpRequestPostAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn, const std::string& strUseData) = 0;

protected:
	virtual bool StartActorPool(const int nCount) = 0;
};

#endif