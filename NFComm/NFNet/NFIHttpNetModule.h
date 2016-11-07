#ifndef NFI_HTTPNET_MODULE_H
#define NFI_HTTPNET_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFCHttpNet.h"

class NFIHttpNetModule
	: public NFIModule
{

public:
	// register msg callback
	template<typename BaseType>
	bool AddReceiveCallBack(const int msgId, BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request*, const int, std::map<std::string, std::string>& argMap))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(msgId, functorPtr);
	}
	template<typename BaseType>
	bool AddNetCommonReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request*, const int, std::map<std::string, std::string>& argMap))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));

		return AddComMsgCB(functorPtr);
	}

public:
	virtual int InitServer(const unsigned short nPort) = 0;
	virtual bool AddMsgCB(const int msgId, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb) = 0;
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb) = 0;
	virtual NFIHttpNet* GetHttpNet() = 0;

public:
};

#endif
