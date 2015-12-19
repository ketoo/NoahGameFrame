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
    Test();
	return true;
}

bool NFCUrlModule::BeforeShut()
{
	return true;
}

int NFCUrlModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    try
    {
        if(strUrl.empty())
        {
            return -2;
        }

        std::string strCookies = CompositeCookies(mxCookies);
        std::string strGetParam = CompositeParam(mxGetParams);
        std::string strPostParam = CompositeParam(mxPostParams);

        std::string strFullStr;

        std::stringstream ss;
        ss<<strUrl.c_str();
        ss<<"?";
        ss<<strGetParam.c_str();
        strFullStr = ss.str();

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

        if (!mstrHost.empty())
        {
            NFSPRINTF(buf, sizeof(buf), "Host: %s", mstrHost.c_str());
            headerlist = curl_slist_append(headerlist, buf);
        }

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
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostParam.c_str());

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

    return 0;
}

int NFCUrlModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    try
    {
        if(strUrl.empty())
        {
            return -2;
        }

        std::string strFullUrl;

        std::string strCookies = CompositeCookies(mxCookies);
        std::string strGetParam = CompositeParam(mxGetParams);

        std::stringstream ss;
        ss<<strUrl.c_str();
        ss<<"?";
        ss<<strUrl.c_str();
        strFullUrl = ss.str();

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

        if (!mstrHost.empty())
        {
            NFSPRINTF(buf, sizeof(buf), "Host: %s", mstrHost.c_str());
            headerlist = curl_slist_append(headerlist, buf);
        }

        StSlistFree slist_free(headerlist);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        char strerrorBuffer[CURL_ERROR_SIZE] = {0};

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, strerrorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, strFullUrl.c_str()); // ok

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RecvHttpData); // ok
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRsp);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1);

        curl_easy_setopt(curl, CURLOPT_COOKIE, strCookies.c_str()); // ok

        if (!strBodyData.empty())
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBodyData.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strBodyData.length());
        }

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (int)(fTimeOutSec*1000));
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, (int)(fTimeOutSec*1000));

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,false);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,false);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK) // CURLE_OK : 0
        {
            return ret;
        }
        else
        {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if(200 != http_code)
            {
                //如果访问正常但http状态码不是200，直接返回http的状态码
                return http_code;
            }

            return 0;//状态码为200时返回0
        }
    }
    catch(...)
    {
        return -1;
    }
    return 0;
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

bool NFCUrlModule::Test()
{
    std::string strUrl;
    std::map<std::string, std::string> mxGetParams;
    std::map<std::string, std::string> mxPostParams;
    std::map<std::string, std::string> mxCookies;
    float fTimeOutSec = 30;
    std::string strRsp;


    strUrl = "http://msdk.qq.com/mpay/get_balance_m";
    mxGetParams["accounttype"]="common";
    mxGetParams["appid"]="1000001494";
    mxGetParams["format"]="json";
    mxGetParams["openid"]="D66D9B77EAEB8699948ED782573F2B42";
    mxGetParams["openkey"]="3A75C477CA25A59372C5ACBD1739EEC5";
    mxGetParams["pay_token"]="A61643B39523D9EDD9BCDA2D2CDB364D";
    mxGetParams["pf"]="desktop_m_qq-2002-android-73213123-qq-1000001494-D66D9B77EAEB8699948ED782573F2B42";
    mxGetParams["pfkey"]="202dd66348036c6006f3cf87761a3750";
    mxGetParams["sig"]="R31slc20TIIwhE%2BDJshp38SvD10%3D";
    mxGetParams["ts"]="1450271625";
    mxGetParams["userip"]="192.168.4.57";
    mxGetParams["zoneid"]="1501";

    mxCookies["session_id"]="openid";
    mxCookies["session_type"]="kp_actoken";
    mxCookies["org_loc"]="/mpay/get_balance_m";
    mxCookies["appip"]="127.0.0.1";

    std::ostringstream osstreamParams;
    osstreamParams<< strUrl <<" http Get Params: ";

    for (std::map<std::string, std::string>::iterator iter = mxGetParams.begin(); iter != mxGetParams.end(); ++iter)
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";

    }

    osstreamParams <<"Post Params: ";
    for (std::map<std::string, std::string>::iterator iter = mxPostParams.begin(); iter != mxPostParams.end(); ++iter)
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";
    }

    osstreamParams <<"Cookies Params: ";
    for (std::map<std::string, std::string>::iterator iter = mxCookies.begin(); iter != mxCookies.end(); iter ++ )
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";
    }


    std::cout << osstreamParams;
    HttpRequest(strUrl, mxGetParams, mxPostParams, mxCookies, fTimeOutSec, strRsp);

    std::cout << "Http Result" << strRsp;

    return false;
}
