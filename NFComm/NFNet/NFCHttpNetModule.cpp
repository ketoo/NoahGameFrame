#include <iostream>
#include "NFCHttpNetModule.h"

NFCHttpNetModule::NFCHttpNetModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mHttpNet = NULL;
}

NFCHttpNetModule::~NFCHttpNetModule()
{
	if (mHttpNet)
	{
		mHttpNet->Final();
	}
	delete mHttpNet;
	mHttpNet = NULL;
}
bool NFCHttpNetModule::Init()
{
	mLogModule = pPluginManager->FindModule<NFILogModule>();
	return true;
}
bool NFCHttpNetModule::Execute()
{
	if (!mHttpNet)
	{
		return false;
	}
	return mHttpNet->Execute();
}

int NFCHttpNetModule::InitServer(const unsigned short nPort)
{
	mHttpNet = new NFCHttpNet(this, &NFCHttpNetModule::OnReceiveNetPack);
	std::ostringstream stream; stream << "Open http port:" << nPort; mLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0,0),stream.str(),"", __FUNCTION__, __LINE__);
	return mHttpNet->InitServer(nPort);
}

//msg callback
void NFCHttpNetModule::OnReceiveNetPack(struct evhttp_request *req, const int msgId, std::map<std::string, std::string>& argMap)
{
	std::map<int, HTTPNET_RECEIVE_FUNCTOR_PTR>::iterator it = mMsgCBMap.find(msgId);
	if (mMsgCBMap.end() != it)
	{
		HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(req, msgId, argMap);
	}
	else
	{
		if (mComMsgCBList.size() > 0)
		{
			for (std::list<HTTPNET_RECEIVE_FUNCTOR_PTR>::iterator it = mComMsgCBList.begin(); it != mComMsgCBList.end(); ++it)
			{
				HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
				HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
				pFunc->operator()(req, msgId, argMap);
			}
		}
		else
		{
			std::cout<< "un support http msgId:" << msgId<<std::endl;
		}

	}
}



//register msg callback
bool NFCHttpNetModule::AddMsgCB(const int msgId, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	if (mMsgCBMap.find(msgId) == mMsgCBMap.end())
	{
		mMsgCBMap.insert(std::map<int, HTTPNET_RECEIVE_FUNCTOR_PTR>::value_type(msgId, cb));
		return true;
	}
	return false;
}

bool NFCHttpNetModule::AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	mComMsgCBList.push_back(cb);
	return true;
}

NFIHttpNet* NFCHttpNetModule::GetHttpNet()
{
	return mHttpNet;
}

