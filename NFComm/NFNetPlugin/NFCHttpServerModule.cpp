#include "NFCHttpServerModule.h"

NFCHttpServerModule::NFCHttpServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mHttpNet = NULL;
}
NFCHttpServerModule::~NFCHttpServerModule()
{
	if (mHttpNet)
	{
		mHttpNet->Final();
		delete mHttpNet;
		mHttpNet = NULL;
	}
}

bool NFCHttpServerModule::Execute()
{
	if(mHttpNet)
		return mHttpNet->Execute();

	return true;
}

int NFCHttpServerModule::InitServer(const unsigned short nPort)
{
	mHttpNet = new NFCHttpNet(this, &NFCHttpServerModule::OnReceiveNetPack);
	std::cout << "Open http port:" << nPort << std::endl;
	return mHttpNet->InitServer(nPort);
}

void NFCHttpServerModule::OnReceiveNetPack(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)
{
	auto it = mMsgCBMap.find(strCommand);
	if (mMsgCBMap.end() != it)
	{
		HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(req, strCommand, strUrl);
	}
	else
	{
		if (mComMsgCBList.size() > 0)
		{
			for (std::list<HTTPNET_RECEIVE_FUNCTOR_PTR>::iterator it = mComMsgCBList.begin(); it != mComMsgCBList.end(); ++it)
			{
				HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
				HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
				pFunc->operator()(req, strCommand, strUrl);
			}
		}
		else
		{
			std::cout << "un support http Command:" << strCommand.c_str() << std::endl;
		}
	}
}

bool NFCHttpServerModule::AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
	{
		mMsgCBMap.insert(std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));
		return true;
	}
	return false;
}

bool NFCHttpServerModule::AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	mComMsgCBList.push_back(cb);
	return true;
}

NFIHttpNet* NFCHttpServerModule::GetHttpNet()
{
	return mHttpNet;
}