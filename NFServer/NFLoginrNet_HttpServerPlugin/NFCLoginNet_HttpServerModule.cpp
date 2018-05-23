// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_HttpServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-01
//    @Module           :    NFCLoginNet_HttpServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFRequestLogin.h"
#include "NFResponseLogin.h"
#include "NFResponseWorldList.h"
#include "NFCLoginNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCLoginNet_HttpServerModule::Init()
{
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLoginServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLoginToMasterModule = pPluginManager->FindModule<NFILoginToMasterModule>();
	
	return true;
}
bool NFCLoginNet_HttpServerModule::Shut()
{
	return true;
}

bool NFCLoginNet_HttpServerModule::AfterInit()
{
	m_pHttpNetModule->AddRequestHandler("/login", NFHttpType::NF_HTTP_REQ_POST, this, &NFCLoginNet_HttpServerModule::OnLogin);
	m_pHttpNetModule->AddRequestHandler("/world", NFHttpType::NF_HTTP_REQ_GET, this, &NFCLoginNet_HttpServerModule::OnWorld);

	m_pHttpNetModule->AddNetFilter("/login", this, &NFCLoginNet_HttpServerModule::OnFilter);
	m_pHttpNetModule->AddNetFilter("/world", this, &NFCLoginNet_HttpServerModule::OnFilter);

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

bool NFCLoginNet_HttpServerModule::Execute()
{
    m_pHttpNetModule->Execute();
	return true;
}

bool NFCLoginNet_HttpServerModule::OnLogin(const NFHttpRequest& req)
{
	std::string strResponse;
	NFResponseLogin xResponsetLogin;

	NFRequestLogin xRequestLogin;
	ajson::load_from_buff(xRequestLogin, req.body.c_str());
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

		//mxConnectkey[xRequestLogin.user] = xRequestLogin.password;

		ajson::string_stream ss;
		ajson::save_to(ss, xResponsetLogin);
		strResponse = ss.str();
	}

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFCLoginNet_HttpServerModule::OnWorld(const NFHttpRequest & req)
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

bool NFCLoginNet_HttpServerModule::OnCommonQuery(const NFHttpRequest& req)
{
	return m_pHttpNetModule->ResponseMsg(req, "OnCommonQuery", NFWebStatus::WEB_ERROR);
}

NFWebStatus NFCLoginNet_HttpServerModule::OnFilter(const NFHttpRequest & req)
{
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
}
