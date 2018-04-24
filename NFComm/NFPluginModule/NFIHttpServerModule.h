// -------------------------------------------------------------------------
//    @FileName      	:   NFIHttpNetModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFIHttpNetModule
//
// -------------------------------------------------------------------------

#ifndef NFI_HTTP_NET_MODULE_H
#define NFI_HTTP_NET_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFNetPlugin/NFIHttpServer.h"

class NFIHttpServerModule
	: public NFIModule
{
public:
	virtual ~NFIHttpServerModule() {};	

	// register msg callback
	template<typename BaseType>
	bool AddReceiveCallBack(const std::string& strPath, BaseType* pBase, bool (BaseType::*handleRecieve)(const NFHttpRequest& req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(strPath, functorPtr);
	}
	template<typename BaseType>
	bool AddNetCommonReceiveCallBack(BaseType* pBase, bool (BaseType::*handleRecieve)(const NFHttpRequest& req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));

		return AddComMsgCB(functorPtr);
	}
	template<typename BaseType>
	bool AddNetFilterCallBack(BaseType* pBase, bool (BaseType::*handleRecieve)(const NFHttpRequest& req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));

		return AddFilterCB(functorPtr);
	}
public:
	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK") = 0;

	virtual bool AddMsgCB(const std::string& strPath, const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;
	virtual bool AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;
	virtual bool AddFilterCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;
};

#endif
