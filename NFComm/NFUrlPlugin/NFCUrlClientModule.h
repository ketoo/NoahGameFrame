// -------------------------------------------------------------------------
//    @FileName      :    NFCUrlClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_URL_MODULE_H_
#define _NFC_URL_MODULE_H_

#include "curl/curl.h"
#include "NFComm/NFPluginModule/NFIUrlModule.h"
#include "NFComm/NFCore/NFIComponent.h"

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

private:
    std::string mstrUrl;
    std::string mstrGetParams;
    std::string mstrBodyData;
    std::string mxCookies;
    float mfTimeOutSec;
    std::string mstrRsp;
};

class NFCUrlClientModule
    : public NFIUrlModule
{
public:
    NFCUrlClientModule(NFIPluginManager* p)
    {
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

    virtual int HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);    
    virtual int HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);    

    virtual int HttpRequestAs(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp);    

    //“Ï≤Ω
    virtual bool StartActorPool(const int nCount);
    virtual bool CloseActorPool();

    int HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);

    static int HttpReq(const std::string& strUrl, const std::string& strGetParams, const std::string& strBodyData,const std::string& mxCookies, const float fTimeOutSec, std::string& strRsp);    

protected:
    static size_t RecvHttpData(void* buffer, size_t size, size_t nmemb, std::string* strRecveData);
    std::string CompositeParam( const std::map<std::string,std::string>& params );
    std::string CompositeCookies( const std::map<std::string,std::string>& params );

protected:
    NFMapEx<int, int> mActorList; //actorid <-->Used
};

#endif