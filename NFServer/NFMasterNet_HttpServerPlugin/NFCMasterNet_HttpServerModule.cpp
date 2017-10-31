#include "NFCMasterNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

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
	m_pHttpNetModule->AddReceiveCallBack("json", this, &NFCMasterNet_HttpServerModule::OnCommandQuery);
	m_pHttpNetModule->AddNetCommonReceiveCallBack(this, &NFCMasterNet_HttpServerModule::OnCommonQuery);

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::HttpServer::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int nJsonPort = m_pElementModule->GetPropertyInt32(strId, NFrame::HttpServer::WebPort());
			int nWebServerAppID = m_pElementModule->GetPropertyInt32(strId, NFrame::HttpServer::ServerID());
			m_strWebRootPath = m_pElementModule->GetPropertyString(strId, NFrame::HttpServer::WebRootPath());

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

void NFCMasterNet_HttpServerModule::OnCommandQuery(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl)
{
	std::string str = m_pMasterServerModule->GetServersStatus();
	m_pHttpNetModule->ResponseMsg(req, str, NFWebStatus::WEB_OK);
}

void NFCMasterNet_HttpServerModule::OnCommonQuery(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl)
{
	m_pHttpNetModule->ResponseFile(req, m_strWebRootPath, "index.html");
}
