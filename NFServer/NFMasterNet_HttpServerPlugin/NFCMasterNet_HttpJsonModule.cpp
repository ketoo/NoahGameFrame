#include "NFCMasterNet_HttpJsonModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

bool NFCMasterNet_HttpJsonModule::Init()
{
	m_pHttpNetModule = new NFIHttpServerModule(pPluginManager);

	return true;
}
bool NFCMasterNet_HttpJsonModule::Shut()
{
	delete m_pHttpNetModule;
	m_pHttpNetModule = nullptr;
	return true;
}

bool NFCMasterNet_HttpJsonModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pMasterServerModule = pPluginManager->FindModule<NFIMasterNet_ServerModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	m_pHttpNetModule->AddReceiveCallBack("json", this, &NFCMasterNet_HttpJsonModule::OnCommandQuery);
	m_pHttpNetModule->AddNetCommonReceiveCallBack(this, &NFCMasterNet_HttpJsonModule::OnCommonQuery);

	int nJsonPort = 80;
	int nWebServerAppID = 0;

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::HttpServer::ThisName());
	if (xLogicClass)
	{
		NFList<std::string>& strIdList = xLogicClass->GetIdList();
		std::string strId;
		for (bool bRet = strIdList.First(strId); bRet; bRet = strIdList.Next(strId))
		{
			nJsonPort = m_pElementModule->GetPropertyInt(strId, NFrame::HttpServer::WebPort());
			nWebServerAppID = m_pElementModule->GetPropertyInt(strId, NFrame::HttpServer::ServerID());
			m_strWebRootPath = m_pElementModule->GetPropertyString(strId, NFrame::HttpServer::WebRootPath());
		}
	}

	//webserver only run one instance for NF
	if (pPluginManager->GetAppID() != nWebServerAppID)
	{
		return true;
	}

	m_pHttpNetModule->InitServer(nJsonPort);

	return true;
}

bool NFCMasterNet_HttpJsonModule::Execute()
{
	m_pHttpNetModule->Execute();
	return true;
}

void NFCMasterNet_HttpJsonModule::OnCommandQuery(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)
{
	std::string str = m_pMasterServerModule->GetServersStatus();
	NFCHttpNet::SendMsg(req, str.c_str());
}

void NFCMasterNet_HttpJsonModule::OnCommonQuery(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)
{
	//Add response type
	std::map<std::string, std::string> typeMap;
	typeMap["txt"] = "text/plain";
	typeMap["txt"] = "text/plain";
	typeMap["c"] = "text/plain";
	typeMap["h"] = "text/plain";
	typeMap["html"] = "text/html";
	typeMap["htm"] = "text/htm";
	typeMap["css"] = "text/css";
	typeMap["gif"] = "image/gif";
	typeMap["jpg"] = "image/jpeg";
	typeMap["jpeg"] = "image/jpeg";
	typeMap["png"] = "image/png";
	typeMap["pdf"] = "application/pdf";
	typeMap["ps"] = "application/postsript";

	std::string strPath = m_strWebRootPath;
	if (strPath.find_last_of("/") != strPath.size())
	{
		strPath += "/";
	}
	strPath = strPath + strUrl;

	int fd = -1;
	struct stat st;
	if (stat(strPath.c_str(), &st) < 0)
	{
		std::string errMsg = "404:" + strPath;
		NFCHttpNet::SendMsg(req, errMsg.c_str());
	}

	if (S_ISDIR(st.st_mode))
	{
		strPath += "/index.html";
	}

	if (stat(strPath.c_str(), &st) < 0)
	{
		std::string errMsg = "404:" + strPath;
		NFCHttpNet::SendMsg(req, errMsg.c_str());
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	if ((fd = open(strPath.c_str(), O_RDONLY | O_BINARY)) < 0) {
#else
	if ((fd = open(strPath.c_str(), O_RDONLY)) < 0) {
#endif
		NFCHttpNet::SendMsg(req, "error");
		return;
	}

	if (fstat(fd, &st) < 0) {
		NFCHttpNet::SendMsg(req, "error");
		return;
	}

	const char* last_period = strrchr(strPath.c_str(), '.');
	std::string strType = last_period + 1;
	if (typeMap.find(strType) == typeMap.end())
	{
		strType = "application/misc";
	}
	else
	{
		strType = typeMap[strType];
	}
	NFCHttpNet::SendFile(req, fd, st, strType);
}
