// -------------------------------------------------------------------------
//    @FileName			:    NFCUrlClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <string>
#include <iostream>
#include <sstream>
#include "NFCUrlClientModule.h"
#include "Dependencies/curl/include/curl/curl.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIUrlCodeModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"


int NFCURLComponent::OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
{
    SURLParam xparam;
    if (!NFCUrlClientModule::UnPackParam(arg, xparam))
    {
        return -1;
    }

    xparam.nRet = NFCUrlClientModule::HttpReq(xparam.strUrl, xparam.strGetParams, xparam.strBodyData, xparam.strCookies, xparam.fTimeOutSec, xparam.strRsp);

    if (!NFCUrlClientModule::PackParam(xparam, arg))
    {
        return -2;
    }

    return 0;
}

NF_SHARE_PTR<NFIComponent> NFCURLComponent::CreateNewInstance()
{
    return  NF_SHARE_PTR<NFIComponent> (NF_NEW  NFCURLComponent(NFGUID(1, 2), ""));
}

bool NFCUrlClientModule::Init()
{
    return true;
}

bool NFCUrlClientModule::Shut()
{
    return true;
}

bool NFCUrlClientModule::Execute()
{
    return true;
}

bool NFCUrlClientModule::AfterInit()
{
    m_pUrlCodeModule = pPluginManager->FindModule<NFIUrlCodeModule>();
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

    if (!mbInitCurl)
    {
        mbInitCurl = true;
        curl_global_init(CURL_GLOBAL_ALL);
    }

	StartActorPool(10);
    return true;
}

bool NFCUrlClientModule::BeforeShut()
{
    return true;
}

int NFCUrlClientModule::HttpPostRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);
    std::string strPostParam = CompositeParam(mxPostParams);
    return HttpReq(strUrl, strGetParam, strPostParam, strCookies, fTimeOutSec, strRsp);
}

int NFCUrlClientModule::HttpRequest(const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, std::string& strRsp)
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
            pStrRecveData->append((char*)buffer, sizes);
        }
        catch (...)
        {
            return 0;
        }
    }

    return sizes;
}

std::string NFCUrlClientModule::CompositeParam(const std::map<std::string, std::string>& params)
{
    std::stringstream ss;
    std::multimap<std::string, std::string>::const_iterator iter = params.begin();
    while (iter != params.end())
    {
        ss << iter->first
           << "="
           << m_pUrlCodeModule->EncodeURIValue(iter->second.c_str())
           << "&";
        ++iter;
    }

    std::string line = ss.str();
    if (line.size() > 1)
    {
        //去掉末尾的&
        line.erase(line.size() - 1);
    }

    return line;
}

std::string NFCUrlClientModule::CompositeCookies(const std::map<std::string, std::string>& params)
{
    std::stringstream ss;
    std::multimap<std::string, std::string>::const_iterator iter = params.begin();
    while (iter != params.end())
    {
        ss << iter->first
           << "="
           << m_pUrlCodeModule->EncodeURIValue(iter->second.c_str())
           << ";";
        ++iter;
    }

    std::string line = ss.str();
    if (line.size() > 1)
    {
        //去掉末尾的;
        line.erase(line.size() - 1);
    }

    return line;
}


int NFCUrlClientModule::HttpReq(const std::string& strUrl, const std::string& strGetParam, const std::string& strBodyData, const std::string& strCookies, const float fTimeOutSec, std::string& strRsp)
{
    try
    {
        if (strUrl.empty())
        {
            return -2;
        }

        std::string strFullStr;
        strFullStr += strUrl;
        strFullStr += "?";
        strFullStr += strGetParam;

        CURL* curl = 0;
        CURLcode ret;

        curl = curl_easy_init();
        if (0 == curl)
        {
            return -3;
        }

        SConnAutoFree conn_free(curl);
        struct curl_slist* headerlist = NULL;

        char buf[256] = {0};

        NFSPRINTF(buf, sizeof(buf), "%s", "Expect:");
        headerlist = curl_slist_append(headerlist, buf);

        SSlistAutoFree slist_free(headerlist);

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

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (int)(fTimeOutSec * 1000));
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, (int)(fTimeOutSec * 1000));

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        if (!strBodyData.empty())
        {
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBodyData.c_str());
        }

        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK) // CURLE_OK : 0
        {
            return ret;
        }
        else
        {
            //如果访问正常但http状态码不是200，直接返回http的状态码
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (200 != http_code)
            {
                return http_code;
            }

            return 0;//状态码为200时返回0
        }
    }
    catch (...)
    {
        return -1;
    }
}

bool NFCUrlClientModule::StartActorPool(const int nCount)
{
    for (int i = 0; i < nCount; i++)
    {
        int nActorID = m_pActorModule->RequireActor<NFCURLComponent>(this, &NFCUrlClientModule::HttpRequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int> (NF_NEW int(nActorID)));
        }
    }

    return true;
}

bool NFCUrlClientModule::CloseActorPool()
{
    int nActor = 0;
    for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
    {
		m_pActorModule->ReleaseActor(nActor);
    }

    mActorList.ClearAll();
    return true;
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

    // std::function 有重载operator bool
    if (!pReqData->mFunRsp)
    {
        return -3;
    }

    pReqData->mFunRsp(self, xResultparam.nRet, xResultparam.strRsp, pReqData->mstrUseData);

    mReqList.RemoveElement(xResultparam.nReqID);

    return 0;
}

int NFCUrlClientModule::HttpRequestAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::string& strBodyData, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn, const std::string& strUseData)
{
    std::string strGetParam = CompositeParam(mxGetParams);
    std::string strCookies = CompositeCookies(mxCookies);

    NF_SHARE_PTR<SURLParam> pUrlParam(NF_NEW SURLParam());
    if (NULL == pUrlParam)
    {
        return -6;
    }

    pUrlParam->strUrl = strUrl;
    pUrlParam->strGetParams = strGetParam;
    pUrlParam->strBodyData = strBodyData;
    pUrlParam->strCookies = strCookies;
    pUrlParam->fTimeOutSec = fTimeOutSec;
    pUrlParam->nReqID = nCurReqID++;
    pUrlParam->mFunRsp = RspFucn;
    pUrlParam->mstrUseData = strUseData;

    int nAcotrID = GetActor();
    if (nAcotrID <= 0)
    {
        return -2;
    }

    std::string arg;
    const int nEvetID = 1;

    if (!PackParam(*pUrlParam, arg))
    {
        return -3;
    }

    if (!mReqList.AddElement(pUrlParam->nReqID, pUrlParam))
    {
        return -4;
    }

    if (!m_pActorModule->SendMsgToActor(nAcotrID, self, nEvetID, arg))
    {
        mReqList.RemoveElement(pUrlParam->nReqID);
        return -5;
    }

    return 0;
}

int NFCUrlClientModule::HttpRequestPostAs(const NFGUID& self, const std::string& strUrl, const std::map<std::string, std::string>& mxGetParams, const std::map<std::string, std::string>& mxPostParams, const std::map<std::string, std::string>& mxCookies, const float fTimeOutSec, const HTTP_RSP_FUNCTOR& RspFucn , const std::string& strUseData)
{
    std::string strPostParam = CompositeParam(mxPostParams);

    return HttpRequestAs(self, strUrl, mxGetParams, strPostParam, mxCookies, fTimeOutSec, RspFucn, strUseData);
}

bool NFCUrlClientModule::PackParam(const SURLParam& xParam, std::string& strData)
{
    try
    {
        NFMsg::PackSURLParam xMsg;

        xMsg.set_strurl(xParam.strUrl);
        xMsg.set_strgetparams(xParam.strGetParams);
        xMsg.set_strbodydata(xParam.strBodyData);
        xMsg.set_strcookies(xParam.strCookies);
        xMsg.set_ftimeoutsec(xParam.fTimeOutSec);
        xMsg.set_strrsp(xParam.strRsp);
        xMsg.set_nret(xParam.nRet);
        xMsg.set_nreqid(xParam.nReqID);

        return xMsg.SerializeToString(&strData);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool NFCUrlClientModule::UnPackParam(const std::string& strData, SURLParam& xParam)
{
    try
    {
        NFMsg::PackSURLParam xMsg;
        if (!xMsg.ParseFromString(strData))
        {
            return false;
        }

        xParam.strUrl           = xMsg.strurl();
        xParam.strGetParams     = xMsg.strgetparams();
        xParam.strBodyData      = xMsg.strbodydata();
        xParam.strCookies       = xMsg.strcookies();
        xParam.strRsp           = xMsg.strrsp();
        xParam.fTimeOutSec      = xMsg.ftimeoutsec();
        xParam.nRet             = xMsg.nret();
        xParam.nReqID           = xMsg.nreqid();

    }
    catch (...)
    {
        return false;
    }

    return true;
}

int NFCUrlClientModule::GetActor()
{
    if (mActorList.Count() <= 0)
    {
        return -1;
    }

    mnSuitIndex ++;
    mnSuitIndex = mnSuitIndex % mActorList.Count();

    int i = 0;
    for (int* pData = mActorList.FirstNude(); pData != NULL ; pData = mActorList.NextNude())
    {
        if (i < mActorList.Count() && i == mnSuitIndex)
        {
            return *pData;
        }

        ++i;
    }


    return -1;
}
