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
#include "NFRequestSelectWorld.h"
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
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	
	return true;
}
bool NFCLoginNet_HttpServerModule::Shut()
{
	return true;
}

bool NFCLoginNet_HttpServerModule::AfterInit()
{
	m_pHttpNetModule->AddRequestHandler("/login", NFHttpType::NF_HTTP_REQ_POST, this, &NFCLoginNet_HttpServerModule::OnLogin);
	m_pHttpNetModule->AddRequestHandler("/world", NFHttpType::NF_HTTP_REQ_GET, this, &NFCLoginNet_HttpServerModule::OnWorldView);
	m_pHttpNetModule->AddRequestHandler("/world", NFHttpType::NF_HTTP_REQ_CONNECT, this, &NFCLoginNet_HttpServerModule::OnWorldSelect);

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

		mToken[xRequestLogin.user] = xGUIDKey.ToString();

		ajson::string_stream ss;
		ajson::save_to(ss, xResponsetLogin);
		strResponse = ss.str();
	}

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFCLoginNet_HttpServerModule::OnWorldView(const NFHttpRequest & req)
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

bool NFCLoginNet_HttpServerModule::OnWorldSelect(const NFHttpRequest & req)
{
	std::string strResponse;
	NFIResponse xResponse;

	std::string user = GetUserID(req);

	NFRequestSelectWorld xRequestSelectWorld;
	ajson::load_from_buff(xRequestSelectWorld, req.body.c_str());
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

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_MASTER, user, NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData);

	YieldCo();

	return m_pHttpNetModule->ResponseMsg(req, strResponse, NFWebStatus::WEB_OK);
}

bool NFCLoginNet_HttpServerModule::OnCommonQuery(const NFHttpRequest& req)
{
	return m_pHttpNetModule->ResponseMsg(req, "OnCommonQuery", NFWebStatus::WEB_ERROR);
}

std::string NFCLoginNet_HttpServerModule::GetUserID(const NFHttpRequest & req)
{
	auto it = req.headers.find("user");
	if (it != req.headers.end())
	{
		return it->second;
	}

	return "";
}

std::string NFCLoginNet_HttpServerModule::GetUserJWT(const NFHttpRequest & req)
{
	auto it = req.headers.find("jwt");
	if (it != req.headers.end())
	{
		return it->second;
	}

	return "";
}

bool NFCLoginNet_HttpServerModule::CheckUserJWT(const std::string & user, const std::string & jwt)
{
	auto it = mToken.find(user);
	if (it != mToken.end())
	{
		return (it->second == jwt);
	}

	return false;
}

NFWebStatus NFCLoginNet_HttpServerModule::OnFilter(const NFHttpRequest & req)
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
