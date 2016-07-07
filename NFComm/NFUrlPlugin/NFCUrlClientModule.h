// -------------------------------------------------------------------------
//    @FileName			:    NFCUrlClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_URL_MODULE_H
#define NFC_URL_MODULE_H

#include "NFComm/NFCore/NFIComponent.h"
#include "Dependencies/curl/include/curl/curl.h"
#include "NFComm/NFPluginModule/NFIUrlClientModule.h"
#include "NFComm/NFPluginModule/NFIUrlCodeModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

struct SURLParam
{
    SURLParam()
    {
        fTimeOutSec = 0.0f;
        nRet = 0;
        nReqID = 0;
        mnUseData = 0;
    }

    std::string strUrl;
    std::string strGetParams;
    std::string strBodyData;
    std::string strCookies;
    float fTimeOutSec;
    std::string strRsp;
    int  nRet;
    int  nReqID;
    HTTP_RSP_FUNCTOR mFunRsp;
    int mnUseData;
    std::string mstrUseData;
};

class NFCURLComponent : public NFIComponent
{
public:
    NFCURLComponent(NFGUID self, const std::string& strName): NFIComponent(self, strName)
    {
    }

    NFCURLComponent(NFIPluginManager* pPluginManager): NFIComponent(NFGUID(), "")
    {
    }

    virtual ~NFCURLComponent()
    {

    }

    virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);

protected:
    virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();
};

class NFCUrlClientModule
    : public NFIUrlClientModule
{
public:
    NFCUrlClientModule(NFIPluginManager* p)
    {
        nCurReqID = 0;
        pPluginManager = p;
        mnSuitIndex = 0;
        mbInitCurl = false;
    }

public:
    struct SConnAutoFree
    {
        SConnAutoFree(CURL* pConn)
        {
            m_pConnect = pConn;
        }
        ~SConnAutoFree()
        {
            if (m_pConnect)
            {
                curl_easy_cleanup(m_pConnect);
                m_pConnect = NULL;
            }
        }
        CURL* m_pConnect;
    };

    struct SSlistAutoFree
    {
        SSlistAutoFree(struct curl_slist* plist)
        {
            m_plist = plist;
        }
        ~SSlistAutoFree()
        {
            if (m_plist)
            {
                curl_slist_free_all(m_plist);
            }
        }
        struct curl_slist* m_plist;
    };

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual int HttpPostRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);
    virtual int HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);
    virtual int HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn,  const std::string& strUseData);
    virtual int HttpRequestPostAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn, const std::string& strUseData);

    virtual bool CloseActorPool();

public:
    static int HttpReq(const std::string& strUrl, const std::string& strGetParams, const std::string& strBodyData, const std::string& mxCookies, const float fTimeOutSec, std::string& strRsp);
    static bool PackParam(const SURLParam& xParam, std::string& strData);
    static bool UnPackParam(const std::string& strData, SURLParam& xParam);

protected:
	virtual bool StartActorPool(const int nCount);
    int GetActor();
    int HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);

    static size_t RecvHttpData(void* buffer, size_t size, size_t nmemb, std::string* strRecveData);
    std::string CompositeParam(const std::map<std::string, std::string>& params);
    std::string CompositeCookies(const std::map<std::string, std::string>& params);

public:
    NFIUrlCodeModule* m_pUrlCodeModule;
protected:
    NFMapEx<int, int> mActorList; //actorid <-->Used
    NFMapEx<int, SURLParam> mReqList;// reqID <-->Param
    int nCurReqID;

    int mnSuitIndex;
    bool mbInitCurl;

	NFIActorModule* m_pActorModule;
};

#endif