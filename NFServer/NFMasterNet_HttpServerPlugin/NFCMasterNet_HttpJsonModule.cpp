#include "NFCMasterNet_HttpJsonModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


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

	m_pHttpNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_QUERY_SERVER_STATUS, this, &NFCMasterNet_HttpJsonModule::OnQueryServerStatus);
	m_pHttpNetModule->AddNetCommonReceiveCallBack(this, &NFCMasterNet_HttpJsonModule::InvalidMessage);


	int nJsonPort = 80;
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::HttpServer::ThisName());
	if (xLogicClass)
	{
		NFList<std::string>& strIdList = xLogicClass->GetIdList();
		std::string strId;
		for (bool bRet = strIdList.First(strId); bRet; bRet = strIdList.Next(strId))
		{
			nJsonPort = m_pElementModule->GetPropertyInt(strId, NFrame::HttpServer::JsonPort());
		}
	}

	m_pHttpNetModule->InitServer(nJsonPort);

	return true;
}

bool NFCMasterNet_HttpJsonModule::Execute()
{
	m_pHttpNetModule->Execute();
	return true;
}

void NFCMasterNet_HttpJsonModule::OnQueryServerStatus(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::string str = m_pMasterServerModule->GetServersStatus();
	
	NFCHttpNet::SendMsg(req, argMap["jsoncallback"] + "(" + str + ");");
}

void NFCMasterNet_HttpJsonModule::InvalidMessage(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::ostringstream stream; stream << "error: unsupport msg";
	NFCHttpNet::SendMsg(req, stream.str());
}
