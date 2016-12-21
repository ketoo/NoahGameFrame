#ifndef NFI_HTTPNET_MODULE_H
#define NFI_HTTPNET_MODULE_H

#include <iostream>
#include "NFComm/NFNet/NFCHttpNet.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFIHttpServerModule
	: public NFIModule
{

public:
	NFIHttpServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
		mHttpNet = NULL;
	}
	~NFIHttpServerModule()
	{
		mHttpNet->Final();
		delete mHttpNet;
		mHttpNet = NULL;
	}
	
	// register msg callback
	template<typename BaseType>
	bool AddReceiveCallBack(const std::string& strCommand, BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(strCommand, functorPtr);
	}
	template<typename BaseType>
	bool AddNetCommonReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));

		return AddComMsgCB(functorPtr);
	}
	
public:
	virtual bool Execute()
	{
		return mHttpNet->Execute();
	}

	virtual int InitServer(const unsigned short nPort)
	{
		mHttpNet = new NFCHttpNet(this, &NFIHttpServerModule::OnReceiveNetPack);
		std::cout << "Open http port:" << nPort << std::endl;
		return mHttpNet->InitServer(nPort);
	}
	
	void OnReceiveNetPack(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)
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
				std::cout<< "un support http Command:" << strCommand.c_str() <<std::endl;
			}
		}	
	}
	
	virtual bool AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
	{
		if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
		{
			mMsgCBMap.insert(std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));
			return true;
		}
		return false;
	}
	
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
	{
		mComMsgCBList.push_back(cb);
		return true;
	}
	
	virtual NFIHttpNet* GetHttpNet()
	{
		return mHttpNet;
	}

private:
	NFILogModule* mLogModule;

	NFIHttpNet* mHttpNet;
	std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
	std::list<HTTPNET_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};

#endif
