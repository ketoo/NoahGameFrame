// -------------------------------------------------------------------------
//    @FileName      :    NFCUrlModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCUrlModule.h"
#include "curl/curl.h"
#include "NFCWebCharacter.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

const std::string NFCURLComponent::GetComponentName() const
{
    return "NFCURLComponent";
}

int NFCURLComponent::OnASyncEvent( const NFGUID& self, const int event, std::string& arg )
{
    return NFCUrlModule::HttpReq(mstrUrl, mstrGetParams, mstrBodyData, mxCookies, mfTimeOutSec, mstrRsp);
}

NF_SHARE_PTR<NFIComponent> NFCURLComponent::CreateNewInstance()
{
  // return  NF_SHARE_PTR<NFIComponent> (NF_NEW  NFCURLComponent(NFGUID(1, 2)));

    return NF_SHARE_PTR<NFIComponent>(NULL);
}

bool NFCUrlModule::Init()
{
	return true;
}

bool NFCUrlModule::Shut()
{
	return true;
}

bool NFCUrlModule::Execute( const float fLasFrametime, const float fStartedTime )
{
	return true;
}

bool NFCUrlModule::AfterInit()
{
	return true;
}

bool NFCUrlModule::BeforeShut()
{
	return true;
}

int NFCUrlModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);
    std::string strPostParam = CompositeParam(mxPostParams);
    return HttpReq(strUrl, strGetParam, strPostParam, strCookies, fTimeOutSec, strRsp);
}

int NFCUrlModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);
    return HttpReq(strUrl, strGetParam, strBodyData, strCookies, fTimeOutSec, strRsp);
}


size_t NFCUrlModule::RecvHttpData(void* buffer, size_t size, size_t nmemb, std::string* pStrRecveData)
{
    unsigned long sizes = size * nmemb;
    if (NULL != pStrRecveData)
    {
        try
        {
            pStrRecveData->append((char *)buffer, sizes);
        }
        catch (...)
        {
            return 0;
        }
    }

    return sizes;
}

std::string NFCUrlModule::CompositeParam( const std::map<std::string,std::string>& params )
{
    std::stringstream ss;
    std::multimap<std::string,std::string>::const_iterator iter = params.begin();
    while( iter != params.end() )
    {
        ss<< iter->first
            <<"="
            << NFCWebCharacter::encodeURIValue(iter->second.c_str())
            <<"&";
        ++iter;
    }

    std::string line = ss.str();
    if (line.size() > 1)
    {//去掉末尾的&
        line.erase( line.size() -1 );
    }

    return line;
}

std::string NFCUrlModule::CompositeCookies( const std::map<std::string,std::string>& params )
{
    std::stringstream ss;
    std::multimap<std::string,std::string>::const_iterator iter = params.begin();
    while( iter != params.end() )
    {
        ss<< iter->first
            <<"="
            << NFCWebCharacter::encodeURIValue(iter->second.c_str())
            <<";";
        ++iter;
    }

    std::string line = ss.str();
    if (line.size() > 1)
    {//去掉末尾的;
        line.erase( line.size() -1 );
    }

    return line;
}


int NFCUrlModule::HttpReq( const std::string& strUrl, const std::string& strGetParam, const std::string& strBodyData,const std::string& strCookies, const float fTimeOutSec, std::string& strRsp )
{
    try
    {
        if(strUrl.empty())
        {
            return -2;
        }

        std::string strFullStr;
        strFullStr += strUrl;
        strFullStr += "?";
        strFullStr += strGetParam;

        CURL *curl = 0;
        CURLcode ret;

        curl = curl_easy_init();
        if (0 == curl)
        {
            return -3;
        }

        StConnFree conn_free(curl);
        struct curl_slist *headerlist=NULL;

        char buf[256] = {0};

        NFSPRINTF(buf, sizeof(buf), "%s", "Expect:");
        headerlist = curl_slist_append(headerlist, buf);

        StSlistFree slist_free(headerlist);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        char strerrorBuffer[CURL_ERROR_SIZE] = {0};

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, strerrorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, strFullStr.c_str()); // ok
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRsp); // ok
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RecvHttpData); // ok

        if (!strCookies.empty())
        {
            curl_easy_setopt(curl, CURLOPT_COOKIE, strCookies.c_str()); // ok
        }

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (int)(fTimeOutSec*1000));
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, (int)(fTimeOutSec*1000));

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,false);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,false);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);


        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBodyData.c_str());

        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK) // CURLE_OK : 0
        {
            return ret;
        }
        else
        {//如果访问正常但http状态码不是200，直接返回http的状态码
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if(200 != http_code)
            {
                return http_code;
            }

            return 0;//状态码为200时返回0
        }
    }
    catch(...)
    {
        return -1;
    }
}

bool NFCUrlModule::StartActorPool( const int nCount )
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    for (int i = 0; i < nCount; i++)
    {
        int nActorID = pActorManager->RequireActor<NFCURLComponent>(this, &NFCUrlModule::HttpRequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int> (NF_NEW int(nActorID)));
        }
    }

    return false;
}

bool NFCUrlModule::CloseActorPool()
{
    int nActor =0;
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
    {
        pActorManager->ReleaseActor(nActor);
    }

    mActorList.ClearAll();
    return false;
}

int NFCUrlModule::HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData)
{
    return 0;
}
