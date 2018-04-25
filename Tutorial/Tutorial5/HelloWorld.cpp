#include "HelloWorld.h"


bool NFCHelloWorld::Init()
{ 
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pHttpClientModule = pPluginManager->FindModule<NFIHttpClientModule>();
	m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
	
    return true;
}

bool NFCHelloWorld::AfterInit()
{
	m_pScheduleModule->AddSchedule(NFGUID(0, 1), "OnHeartBeat", this, &NFCHelloWorld::OnHeartBeat, 5.0f, 10);

	std::cout << "Hello, world, Init" << std::endl;
	//http://127.0.0.1/json
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_GET, this, &NFCHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_POST, this, &NFCHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_DELETE, this, &NFCHelloWorld::OnCommandQuery);
	m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_PUT, this, &NFCHelloWorld::OnCommandQuery);

	m_pHttpNetModule->AddNetDefaultHandler(this, &NFCHelloWorld::OnCommonQuery);
	m_pHttpNetModule->AddNetFilter(this, &NFCHelloWorld::OnFilter);

	m_pHttpNetModule->InitServer(8080);

    return true;
}

bool NFCHelloWorld::Execute()
{
 
    return true;
}

bool NFCHelloWorld::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFCHelloWorld::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}

bool NFCHelloWorld::OnCommandQuery(const NFHttpRequest & req)
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

bool NFCHelloWorld::OnCommonQuery(const NFHttpRequest & req)
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


	return m_pHttpNetModule->ResponseMsg(req, "OnCommandQuery +++ test2", NFWebStatus::WEB_OK);
}

NFWebStatus NFCHelloWorld::OnFilter(const NFHttpRequest & req)
{
	std::cout << "OnFilter ... " << std::endl;

	return NFWebStatus::WEB_OK;
}

int NFCHelloWorld::OnHeartBeat(const NFGUID & self, const std::string & strHeartBeat, const float fTime, const int nCount)
{
	m_pHttpClientModule->DoGet("http://192.168.13.133:8080/json", this, &NFCHelloWorld::OnGetCallBack);
	m_pHttpClientModule->DoPost("http://192.168.13.133:8080/json", "OnHeartBeat post data---", this, &NFCHelloWorld::OnPostCallBack);

	return 0;
}

void NFCHelloWorld::OnGetCallBack(const NFGUID id, const int state_code, const std::string & strRespData)
{
	std::cout << "OnGetCallBack" << std::endl;
}

void NFCHelloWorld::OnPostCallBack(const NFGUID id, const int state_code, const std::string & strRespData)
{
	std::cout << "OnPostCallBack" << std::endl;
}
