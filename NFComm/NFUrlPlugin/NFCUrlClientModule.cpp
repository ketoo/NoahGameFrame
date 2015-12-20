// -------------------------------------------------------------------------
//    @FileName      :    NFCUrlClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCUrlClientModule.h"
#include "curl/curl.h"
#include "NFCWebCharacter.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

const std::string NFCURLComponent::GetComponentName() const
{
    return "NFCURLComponent";
}

int NFCURLComponent::OnASyncEvent( const NFGUID& self, const int event, std::string& arg )
{
    SURLParam xparam;
    if (!NFCUrlClientModule::UnPackParam(arg, xparam))
    {
        return -1;
    }

    xparam.nRet = NFCUrlClientModule::HttpReq(xparam.strUrl, xparam.strGetParams, xparam.strBodyData, xparam.xCookies, xparam.fTimeOutSec, xparam.strRsp);

    if (!NFCUrlClientModule::PackParam(xparam, arg))
    {
        return -2;
    }

    return 0;
}

NF_SHARE_PTR<NFIComponent> NFCURLComponent::CreateNewInstance()
{
  return  NF_SHARE_PTR<NFIComponent> (NF_NEW  NFCURLComponent(NFGUID(1, 2)));
}

bool NFCUrlClientModule::Init()
{
	return true;
}

bool NFCUrlClientModule::Shut()
{
	return true;
}

bool NFCUrlClientModule::Execute( const float fLasFrametime, const float fStartedTime )
{
	return true;
}

bool NFCUrlClientModule::AfterInit()
{
	return true;
}

bool NFCUrlClientModule::BeforeShut()
{
	return true;
}

int NFCUrlClientModule::HttpPostRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);
    std::string strPostParam = CompositeParam(mxPostParams);
    return HttpReq(strUrl, strGetParam, strPostParam, strCookies, fTimeOutSec, strRsp);
}

int NFCUrlClientModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);
    return HttpReq(strUrl, strGetParam, strBodyData, strCookies, fTimeOutSec, strRsp);
}


size_t NFCUrlClientModule::RecvHttpData(void* buffer, size_t size, size_t nmemb, std::string* pStrRecveData)
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

std::string NFCUrlClientModule::CompositeParam( const std::map<std::string,std::string>& params )
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

std::string NFCUrlClientModule::CompositeCookies( const std::map<std::string,std::string>& params )
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


int NFCUrlClientModule::HttpReq( const std::string& strUrl, const std::string& strGetParam, const std::string& strBodyData,const std::string& strCookies, const float fTimeOutSec, std::string& strRsp )
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

bool NFCUrlClientModule::StartActorPool( const int nCount )
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    for (int i = 0; i < nCount; i++)
    {
        int nActorID = pActorManager->RequireActor<NFCURLComponent>(this, &NFCUrlClientModule::HttpRequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int> (NF_NEW int(nActorID)));
        }
    }

    return false;
}

bool NFCUrlClientModule::CloseActorPool()
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

int NFCUrlClientModule::HttpRequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData)
{
    SURLParam xResultparam;
    if (!NFCUrlClientModule::UnPackParam(strData, xResultparam))
    {
        return -1;
    }

    NF_SHARE_PTR<SURLParam> pReqData = mReqList.GetElement(xResultparam.nReqID);
    if (NULL == pReqData)
    {
        return -2;
    }

    if (pReqData->mFunRsp._Empty())
    {
        return -3;
    }

    pReqData->mFunRsp(self, xResultparam.nRet, xResultparam.strRsp);

    return 0;
}

int NFCUrlClientModule::HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);

    NF_SHARE_PTR<SURLParam> pUrlParam(NF_NEW SURLParam());
    if (NULL == pUrlParam)
    {
        return false;
    }

    pUrlParam->strUrl = strUrl;
    pUrlParam->strGetParams = strGetParam;
    pUrlParam->strBodyData = strBodyData;
    pUrlParam->xCookies = strCookies;
    pUrlParam->fTimeOutSec = fTimeOutSec;
    pUrlParam->nReqID = nCurReqID++;
    pUrlParam->mFunRsp = RspFucn;

    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return -1;
    }

    int nAcotrID = GetActor();
    if (nAcotrID <=0 )
    {
        return -2;
    }

    std::string arg;
    const int nEvetID =1;

    if (!PackParam(*pUrlParam, arg))
    {
        return -3;
    }

    if (!mReqList.AddElement(pUrlParam->nReqID, pUrlParam))
    {
        return -4;
    }

    if (!pActorManager->SendMsgToActor(nAcotrID, self, nEvetID, arg))
    {
        mReqList.RemoveElement(pUrlParam->nReqID);
        return -5;
    }

    return 0;
}

int NFCUrlClientModule::HttpRequestPostAs( const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams,const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn )
{
    std::string strPostParam = CompositeParam(mxPostParams);

    return HttpRequestAs(self, strUrl, mxGetParams, strPostParam, mxCookies, fTimeOutSec, RspFucn);
}

bool NFCUrlClientModule::PackParam( const SURLParam& xParam, std::string& strData )
{
    try
    {
        msgpack::type::tuple<std::string, std::string, std::string, std::string, std::string, float, int, int> xPackMode(xParam.strUrl, xParam.strGetParams, xParam.strBodyData, xParam.xCookies, xParam.strRsp, xParam.fTimeOutSec, xParam.nRet, xParam.nReqID);

        std::stringstream buffer;
        msgpack::pack(buffer, xPackMode);
        buffer.seekg(0);
        strData.assign(buffer.str());
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool NFCUrlClientModule::UnPackParam( const std::string& strData, SURLParam& xParam )
{
    try
    {
        msgpack::unpacked result;

        msgpack::unpack(result, strData.data(), strData.size());
        msgpack::object deserialized = result.get();

        msgpack::type::tuple<std::string, std::string, std::string, std::string, std::string, float, int, int> dst;
        deserialized.convert(&dst);

        xParam.strUrl = dst.a0;
        xParam.strGetParams = dst.a1;
        xParam.strBodyData = dst.a2;
        xParam.xCookies = dst.a3;
        xParam.strRsp = dst.a4;
        xParam.fTimeOutSec = dst.a5;
        xParam.nRet = dst.a6;
        xParam.nReqID = dst.a7;

    }
    catch(...)
    {
        return false;
    }
    
    return true;
}

int NFCUrlClientModule::GetActor()
{
    int nActor = 0;
    NF_SHARE_PTR<int> pdata = mActorList.Next(nActor);
    if (NULL != pdata)
    {
        return nActor;
    }

    pdata = mActorList.First(nActor);
    if (NULL != pdata)
    {
        return nActor;
    }

    return -1;
}
