#include "NFCMasterNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCMasterNet_HttpServerModule::Init()
{
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pMasterServerModule = pPluginManager->FindModule<NFIMasterNet_ServerModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	return true;
}
bool NFCMasterNet_HttpServerModule::Shut()
{
	return true;
}

bool NFCMasterNet_HttpServerModule::AfterInit()
{
    //http://127.0.0.1/json
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_GET, this, &NFCMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_POST, this, &NFCMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_DELETE, this, &NFCMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_PUT, this, &NFCMasterNet_HttpServerModule::OnCommandQuery);

	m_pHttpNetModule->AddNetDefaultHandler(this, &NFCMasterNet_HttpServerModule::OnCommonQuery);
	m_pHttpNetModule->AddNetFilter(this, &NFCMasterNet_HttpServerModule::OnFilter);

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

bool NFCMasterNet_HttpServerModule::Execute()
{
    m_pHttpNetModule->Execute();
	return true;
}

bool NFCMasterNet_HttpServerModule::OnCommandQuery(const NFHttpRequest& req)
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

	std::string str = m_pMasterServerModule->GetServersStatus();
	return m_pHttpNetModule->ResponseMsg(req, str, NFWebStatus::WEB_OK);
}

bool NFCMasterNet_HttpServerModule::OnCommonQuery(const NFHttpRequest& req)
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

	return m_pHttpNetModule->ResponseMsg(req, "OnCommonQuery", NFWebStatus::WEB_OK);
}

NFWebStatus NFCMasterNet_HttpServerModule::OnFilter(const NFHttpRequest & req)
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
