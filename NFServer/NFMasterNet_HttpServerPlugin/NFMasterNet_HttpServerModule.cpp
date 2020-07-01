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

#include "NFMasterNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFMasterNet_HttpServerModule::Init()
{
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pMasterServerModule = pPluginManager->FindModule<NFIMasterNet_ServerModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	return true;
}
bool NFMasterNet_HttpServerModule::Shut()
{
	return true;
}

bool NFMasterNet_HttpServerModule::AfterInit()
{
    //http://127.0.0.1/json
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_GET, this, &NFMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_POST, this, &NFMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_DELETE, this, &NFMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_PUT, this, &NFMasterNet_HttpServerModule::OnCommandQuery);

	m_pHttpNetModule->AddNetFilter("/json", this, &NFMasterNet_HttpServerModule::OnFilter);

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int nJsonPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::WebPort());
			int nWebServerAppID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());

			//webserver only run one instance in each server
			if (pPluginManager->GetAppID() == nWebServerAppID)
			{
				m_pHttpNetModule->InitServer(nJsonPort);

				break;
			}
		}
	}

	return true;
}

bool NFMasterNet_HttpServerModule::Execute()
{
    //m_pHttpNetModule->Execute();
	return true;
}

bool NFMasterNet_HttpServerModule::OnCommandQuery(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::cout << "url: " << req->url << std::endl;
	std::cout << "path: " << req->path << std::endl;
	std::cout << "type: " << req->type << std::endl;
	std::cout << "body: " << req->body << std::endl;

	std::cout << "params: " << std::endl;

	for (auto item : req->params)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

    std::cout << "headers: " << std::endl;

	for (auto item : req->headers)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

	std::string str = m_pMasterServerModule->GetServersStatus();
	return m_pHttpNetModule->ResponseMsg(req, str, NFWebStatus::WEB_OK);
}

NFWebStatus NFMasterNet_HttpServerModule::OnFilter(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::cout << "OnFilter: " << std::endl;

	std::cout << "url: " << req->url << std::endl;
	std::cout << "path: " << req->path << std::endl;
	std::cout << "type: " << req->type << std::endl;
	std::cout << "body: " << req->body << std::endl;

	std::cout << "params: " << std::endl;

	for (auto item : req->params)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

	std::cout << "headers: " << std::endl;

	for (auto item : req->headers)
	{
		std::cout << item.first << ":" << item.second << std::endl;
	}

	return NFWebStatus::WEB_OK;
}
