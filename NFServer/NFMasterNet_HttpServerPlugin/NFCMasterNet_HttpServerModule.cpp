#include "NFCMasterNet_HttpServerModule.h"



bool NFCMasterNet_HttpServerModule::Init()
{
	mHttpNetModule = new NFCHttpNetModule(pPluginManager);

	mKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	mMasterServerModule = pPluginManager->FindModule<NFIMasterNet_ServerModule>();
	return true;
}
bool NFCMasterNet_HttpServerModule::Shut()
{
	return true;
}

bool NFCMasterNet_HttpServerModule::AfterInit()
{
	mHttpNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_QUERY_SERVER_STATUS, this, &NFCMasterNet_HttpServerModule::OnQueryServerStatus);
	mHttpNetModule->AddNetCommonReceiveCallBack(this, &NFCMasterNet_HttpServerModule::InvalidMessage);

	mHttpNetModule->InitServer(7000);
	return true;
}

bool NFCMasterNet_HttpServerModule::Execute()
{
	mHttpNetModule->Execute();
	return true;
}

void NFCMasterNet_HttpServerModule::OnQueryServerStatus(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::string str = mMasterServerModule->GetServersStatus();
	NFCHttpNet::SendMsg(req, str);
}

void NFCMasterNet_HttpServerModule::InvalidMessage(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::ostringstream stream; stream << "error: unsupport msg";
	NFCHttpNet::SendMsg(req, stream.str());
}
