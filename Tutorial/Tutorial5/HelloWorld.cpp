/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "HelloWorld.h"


bool NFHelloWorld::Init()
{ 
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pHttpClientModule = pPluginManager->FindModule<NFIHttpClientModule>();
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	
    return true;
}

bool NFHelloWorld::AfterInit()
{
	m_pScheduleModule->AddSchedule(NFGUID(0, 1), "OnHeartBeat1", this, &NFHelloWorld::OnHeartBeat, 5.0f, 10);
	m_pScheduleModule->AddSchedule(NFGUID(0, 1), "OnHeartBeat2", this, &NFHelloWorld::OnHeartBeat, 5.0f, 10);

	std::cout << "Hello, world, Init" << std::endl;
	//http://127.0.0.1/json
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_GET, this, &NFHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_POST, this, &NFHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_DELETE, this, &NFHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_PUT, this, &NFHelloWorld::OnCommandQuery);

	m_pHttpNetModule->AddNetFilter("/json", this, &NFHelloWorld::OnFilter);

	m_pHttpNetModule->InitServer(8080);

    return true;
}

bool NFHelloWorld::Execute()
{
 
    return true;
}

bool NFHelloWorld::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFHelloWorld::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}

bool NFHelloWorld::OnCommandQuery(const NFHttpRequest & req)
{
	std::cout << "url: " << req.url << std::endl;
	std::cout << "path: " << req.path << std::endl;
	std::cout << "type: " << req.type << std::endl;
	std::cout << "body: " << req.body << std::endl;

	std::cout << "params: " << std::endl;

	for (auto item : req.params)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

	std::cout << "headers: " << std::endl;

	for (auto item : req.headers)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

	return m_pHttpNetModule->ResponseMsg(req, "OnCommandQuery --- test1", NFWebStatus::WEB_OK);
}

NFWebStatus NFHelloWorld::OnFilter(const NFHttpRequest & req)
{
	std::cout << "OnFilter ... " << std::endl;

	return NFWebStatus::WEB_OK;
}

int NFHelloWorld::OnHeartBeat(const NFGUID & self, const std::string & strHeartBeat, const float fTime, const int nCount)
{
	std::cout << strHeartBeat << std::endl;

	m_pHttpClientModule->DoGet("http://127.0.0.1:8080/json", this, &NFHelloWorld::OnGetCallBack);
	m_pHttpClientModule->DoGet("http://127.0.0.1:8080/json", [](const NFGUID id, const int state_code, const std::string & strRespData, const std::string & strMemoData) -> void
	{
		std::cout << "OnGetCallBack" << std::endl;
	});

    std::string strMemo = "Memo here";
	m_pHttpClientModule->DoPost("http://127.0.0.1:8080/json", "OnHeartBeat post data---", this, &NFHelloWorld::OnPostCallBack, strMemo);

	m_pHttpClientModule->DoPost("http://127.0.0.1:8080/json", "OnHeartBeat post data---", [](const NFGUID id, const int state_code, const std::string & strRespData, const std::string & strMemoData) -> void
	{
		std::cout << "OnPostCallBack" << std::endl;
	});

	return 0;
}

void NFHelloWorld::OnGetCallBack(const NFGUID id, const int state_code, const std::string & strRespData)
{
	std::cout << "OnGetCallBack" << std::endl;
}

void NFHelloWorld::OnPostCallBack(const NFGUID id, const int state_code, const std::string& strRespData, const std::string& strMemoData)
{
    std::cout << "OnPostCallBack" << " "<< strMemoData <<std::endl;
}
