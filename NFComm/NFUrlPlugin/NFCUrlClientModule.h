// -------------------------------------------------------------------------
//    @FileName      :    NFCUrlClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_URL_MODULE_H
#define NFC_URL_MODULE_H

#include "curl/curl.h"
#include "NFComm/NFPluginModule/NFIUrlClientModule.h"
#include "NFComm/NFCore/NFIComponent.h"

struct SURLParam
{
    SURLParam()
    {
        fTimeOutSec = 0.0f;
        nRet = 0;
        nReqID = 0;
    }

    std::string strUrl;
    std::string strGetParams;
    std::string strBodyData;
    std::string xCookies;
    float fTimeOutSec;
    std::string strRsp;
    int  nRet;
    int  nReqID;
    HTTP_RSP_FUNCTOR mFunRsp;
};

class NFCURLComponent : public NFIComponent
{
public:
    NFCURLComponent(NFGUID self):NFIComponent(self)
    {
    }

    NFCURLComponent(NFIPluginManager* pPluginManager):NFIComponent(NFGUID(0,0))
    {
    }

    virtual ~NFCURLComponent()
    {

    }
    
    virtual const std::string GetComponentName() const ;
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
    }

public:
    struct StConnFree
    {
        StConnFree(CURL* conn)
        {
            m_conn = conn;
        }
        ~StConnFree()
        {
            if (m_conn)
            {
                curl_easy_cleanup(m_conn);
                m_conn = NULL;
            }
        }
        CURL* m_conn;
    };

    struct StSlistFree
    {
        StSlistFree(struct curl_slist *list)
        {
            m_list = list;
        }
        ~StSlistFree()
        {
            if (m_list)
            {
                curl_slist_free_all (m_list);
            }
        }
        struct curl_slist *m_list;
    };

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual int HttpPostRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);    
    virtual int HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);    
    virtual int HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn);
    virtual int HttpRequestPostAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn);

    virtual bool StartActorPool(const int nCount);
    virtual bool CloseActorPool();

public:
    static int HttpReq(const std::string& strUrl, const std::string& strGetParams, const std::string& strBodyData,const std::string& mxCookies, const float fTimeOutSec, std::string& strRsp);    
    static bool PackParam(const SURLParam& xParam, std::string& strData);
    static bool UnPackParam(const std::string& strData, SURLParam& xParam);

protected:
    int GetActor();
    int HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);

    static size_t RecvHttpData(void* buffer, size_t size, size_t nmemb, std::string* strRecveData);
    std::string CompositeParam( const std::map<std::string,std::string>& params );
    std::string CompositeCookies( const std::map<std::string,std::string>& params );

protected:
    NFMapEx<int, int> mActorList; //actorid <-->Used
    NFMapEx<int, SURLParam> mReqList;// reqID <-->Param
    int nCurReqID;
};

#endif