/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFHttpClientModule.h"
#include "NFHttpClient.h"

#define DEFAULT_USER_AGENT "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Mobile Safari/537.36"

NFHttpClientModule::NFHttpClientModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
    pPluginManager = p;
    m_pHttpClient = new NFHttpClient();
    m_xDefaultHttpHeaders["Connection"] = "close";
    //evhttp_add_header(output_headers, "Connection", "keep-alive");
	m_xDefaultHttpHeaders["Content-Type"] = "text/plain;text/html;application/x-www-form-urlencoded;charset=utf-8";
	//m_xDefaultHttpHeaders["Content-Type"] = "text/plain;text/html;application/x-www-form-urlencoded;charset=utf-8";
    m_xDefaultHttpHeaders["User-Agent"] = DEFAULT_USER_AGENT;
    m_xDefaultHttpHeaders["Cache-Control"] = "no-cache";
}

NFHttpClientModule::~NFHttpClientModule()
{
    delete m_pHttpClient;
    m_pHttpClient = NULL;
}

bool NFHttpClientModule::Init()
{
    m_pHttpClient->Init();

    return true;
}

bool NFHttpClientModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	return true;
}

bool NFHttpClientModule::Execute()
{
    m_pHttpClient->Execute();//TODO
    return true;
}

bool NFHttpClientModule::Shut()
{
    m_pHttpClient->Final();

    return true;
}

int NFHttpClientModule::Post(const std::string & strUri, const std::string & strData, std::string & strResData)
{
	return Post(strUri, m_xDefaultHttpHeaders, strData, strResData);
}

int NFHttpClientModule::Post(const std::string & strUri, const std::map<std::string, std::string>& xHeaders, const std::string & strData, std::string & strResData)
{
	HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(&NFHttpClientModule::CallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	NFGUID id = m_pKernelModule->CreateGUID();
    std::string memo;
    m_pHttpClient->DoPost(strUri, strData,memo, pd, xHeaders, id);

	mxRespDataMap.AddElement(id, NF_SHARE_PTR<RespData>(NF_NEW RespData()));

	NF_SHARE_PTR<RespData> xRespData = mxRespDataMap.GetElement(id);
	while (!xRespData->resp)
	{
        NFSLEEP(1);
	}

	strResData = xRespData->strRespData;

	return xRespData->state_code;
}

int NFHttpClientModule::Get(const std::string & strUri, std::string & strResData)
{
	return Get(strUri, m_xDefaultHttpHeaders, strResData);
}

int NFHttpClientModule::Get(const std::string & strUri, const std::map<std::string, std::string>& xHeaders, std::string & strResData)
{
	HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(&NFHttpClientModule::CallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	NFGUID id = m_pKernelModule->CreateGUID();
	m_pHttpClient->DoGet(strUri, pd, m_xDefaultHttpHeaders, id);

	mxRespDataMap.AddElement(id, NF_SHARE_PTR<RespData>(NF_NEW RespData()));

	NF_SHARE_PTR<RespData> xRespData = mxRespDataMap.GetElement(id);
	while (!xRespData->resp)
	{
        NFSLEEP(1);
	}

	strResData = xRespData->strRespData;

	return xRespData->state_code;
}

bool NFHttpClientModule::DoGet(const std::string& strUri,
                                     const std::map<std::string, std::string>& xHeaders,
                                     HTTP_RESP_FUNCTOR_PTR pCB)
{
    return m_pHttpClient->DoGet(strUri, pCB, xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}

bool NFHttpClientModule::DoPost(const std::string& strUri,
                                      const std::map<std::string, std::string>& xHeaders,
                                      const std::string& strPostData,
                                      HTTP_RESP_FUNCTOR_PTR pCB, const std::string& strMemo)
{
    NFGUID aid = m_pKernelModule->CreateGUID();

    return m_pHttpClient->DoPost(strUri, strPostData, strMemo, pCB,
        xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders, aid);

}

void NFHttpClientModule::CallBack(const NFGUID id, const int state_code, const std::string & strRespData)
{
	NF_SHARE_PTR<RespData> xRespData = mxRespDataMap.GetElement(id);
	if (xRespData)
	{
		xRespData->resp = true;
		xRespData->state_code = state_code;
		xRespData->strRespData = strRespData;
	}
}
