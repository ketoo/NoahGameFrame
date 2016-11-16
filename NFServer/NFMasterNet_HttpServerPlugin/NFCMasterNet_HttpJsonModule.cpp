#include "NFCMasterNet_HttpJsonModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


bool NFCMasterNet_HttpJsonModule::Init()
{
	mHttpNetModule = new NFIHttpServerModule(pPluginManager);

	mKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	mMasterServerModule = pPluginManager->FindModule<NFIMasterNet_ServerModule>();
	return true;
}
bool NFCMasterNet_HttpJsonModule::Shut()
{
	return true;
}

bool NFCMasterNet_HttpJsonModule::AfterInit()
{
	mHttpNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_QUERY_SERVER_STATUS, this, &NFCMasterNet_HttpJsonModule::OnQueryServerStatus);
	mHttpNetModule->AddNetCommonReceiveCallBack(this, &NFCMasterNet_HttpJsonModule::InvalidMessage);

	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

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
	mHttpNetModule->InitServer(nJsonPort);
	return true;
}

bool NFCMasterNet_HttpJsonModule::Execute()
{
	mHttpNetModule->Execute();
	return true;
}

void NFCMasterNet_HttpJsonModule::OnQueryServerStatus(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::string str = mMasterServerModule->GetServersStatus();
	
	NFCHttpNet::SendMsg(req, argMap["jsoncallback"] + "(" + str + ");");
}

void NFCMasterNet_HttpJsonModule::InvalidMessage(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::ostringstream stream; stream << "error: unsupport msg";
	NFCHttpNet::SendMsg(req, stream.str());
}
