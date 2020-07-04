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

#include "NFRequestLogin.h"
#include "NFResponseLogin.h"
#include "NFResponseWorldList.h"
#include "NFRequestSelectWorld.h"
#include "NFLoginNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFLoginNet_HttpServerModule::Init()
{
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLoginServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLoginToMasterModule = pPluginManager->FindModule<NFILoginToMasterModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	
	return true;
}
bool NFLoginNet_HttpServerModule::Shut()
{
	return true;
}

bool NFLoginNet_HttpServerModule::AfterInit()
{
	m_pHttpNetModule->AddRequestHandler("/login", NFHttpType::NF_HTTP_REQ_POST, this, &NFLoginNet_HttpServerModule::OnLogin);
	m_pHttpNetModule->AddRequestHandler("/world", NFHttpType::NF_HTTP_REQ_GET, this, &NFLoginNet_HttpServerModule::OnWorldView);
	m_pHttpNetModule->AddRequestHandler("/world", NFHttpType::NF_HTTP_REQ_CONNECT, this, &NFLoginNet_HttpServerModule::OnWorldSelect);

	m_pHttpNetModule->AddNetFilter("/world", this, &NFLoginNet_HttpServerModule::OnFilter);

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
			if (pPluginManager->GetAppID() == nWebServerAppID && nJsonPort > 0)
			{
				m_pHttpNetModule->InitServer(nJsonPort);

				break;
			}
		}
	}

	return true;
}

bool NFLoginNet_HttpServerModule::Execute()
{
    //m_pHttpNetModule->Execute();
	return true;
}

bool NFLoginNet_HttpServerModule::OnLogin(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::string strResponse;
	NFResponseLogin xResponsetLogin;

	NFRequestLogin xRequestLogin;
	ajson::load_from_buff(xRequestLogin, req->body.c_str());
	if (xRequestLogin.user.empty()
		|| xRequestLogin.password.empty())
	{
		xResponsetLogin.code = NFIResponse::ResponseType::RES_TYPE_FAILED;
		xResponsetLogin.jwt = "";

		ajson::string_stream ss;
		ajson::save_to(ss, xResponsetLogin);
		strResponse = ss.str();
	}
	else
	{
		NFGUID xGUIDKey = m_pKernelModule->CreateGUID();

		xResponsetLogin.code = NFIResponse::ResponseType::RES_TYPE_SUCCESS;
		xResponsetLogin.jwt = xGUIDKey.ToString();

		mToken[xRequestLogin.user] = xGUIDKey.ToString();

		ajson::string_stream ss;
		ajson::save_to(ss, xResponsetLogin);
		strResponse = ss.str();
	}

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFLoginNet_HttpServerModule::OnWorldView(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::string strResponse;
	NFResponseWorldList xResponsetWorldList;

	NFMapEx<int, NFMsg::ServerInfoReport>& xWorldMap = m_pLoginToMasterModule->GetWorldMap();
	NFMsg::ServerInfoReport* pWorldData = xWorldMap.FirstNude();
	while (pWorldData)
	{
		NFResponseWorldList::NFWorld xWorld;

		xWorld.id = pWorldData->server_id();
		xWorld.name = pWorldData->server_name();
		xWorld.state = pWorldData->server_state();
		xWorld.count = pWorldData->server_cur_count();

		xResponsetWorldList.world.push_back(xWorld);

		pWorldData = xWorldMap.NextNude();
	}

	ajson::string_stream ss;
	ajson::save_to(ss, xResponsetWorldList);
	strResponse = ss.str();

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFLoginNet_HttpServerModule::OnWorldSelect(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::string strResponse;
	NFIResponse xResponse;

	std::string user = GetUserID(req);

	NFRequestSelectWorld xRequestSelectWorld;
	ajson::load_from_buff(xRequestSelectWorld, req->body.c_str());
	if (xRequestSelectWorld.id == 0)
	{
		xResponse.code = NFIResponse::ResponseType::RES_TYPE_FAILED;

		ajson::string_stream ss;
		ajson::save_to(ss, xResponse);
		strResponse = ss.str();

		return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
	}

	NFMsg::ReqConnectWorld xData;
	xData.set_world_id(xRequestSelectWorld.id);
	xData.set_login_id(pPluginManager->GetAppID());
	xData.mutable_sender()->CopyFrom(NFINetModule::NFToPB(NFGUID()));
	xData.set_account(user);

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_MASTER, user, NFMsg::EGameMsgID::REQ_CONNECT_WORLD, xData);
	/*
	while (true)
	{
		YieldCo();

		if ()
		{
			break;
		}
	}
	*/

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFLoginNet_HttpServerModule::OnCommonQuery(NF_SHARE_PTR<NFHttpRequest> req)
{
	return m_pHttpNetModule->ResponseMsg(req, "OnCommonQuery", NFWebStatus::WEB_ERROR);
}

std::string NFLoginNet_HttpServerModule::GetUserID(NF_SHARE_PTR<NFHttpRequest> req)
{
	auto it = req->headers.find("user");
	if (it != req->headers.end())
	{
		return it->second;
	}

	return "";
}

std::string NFLoginNet_HttpServerModule::GetUserJWT(NF_SHARE_PTR<NFHttpRequest> req)
{
	auto it = req->headers.find("jwt");
	if (it != req->headers.end())
	{
		return it->second;
	}

	return "";
}

bool NFLoginNet_HttpServerModule::CheckUserJWT(const std::string & user, const std::string & jwt)
{
	auto it = mToken.find(user);
	if (it != mToken.end())
	{
		return (it->second == jwt);
	}

	return false;
}

NFWebStatus NFLoginNet_HttpServerModule::OnFilter(NF_SHARE_PTR<NFHttpRequest> req)
{
	std::string user = GetUserID(req);
	std::string jwt = GetUserJWT(req);

	bool bRet = CheckUserJWT(user, jwt);
	if (bRet)
	{
		return NFWebStatus::WEB_OK;
	}

	return NFWebStatus::WEB_AUTH;
	/*
	std::cout << "OnFilter: " << std::endl;

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

	return NFWebStatus::WEB_OK;
	*/
}
