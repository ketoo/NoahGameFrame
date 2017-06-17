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
#include "NFComm/NFNetPlugin/NFIHttpNet.h"

class NFIHttpNetModule
	: public NFIModule
{
public:
	virtual ~NFIHttpNetModule() {};	

	// register msg callback
	template<typename BaseType>
	bool AddReceiveCallBack(const std::string& strCommand, BaseType* pBase, void (BaseType::*handleRecieve)(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(strCommand, functorPtr);
	}
	template<typename BaseType>
	bool AddNetCommonReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));

		return AddComMsgCB(functorPtr);
	}
	
public:
	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool SendMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK") = 0;
	virtual bool SendFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName) = 0;

	virtual bool AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;	
	virtual bool AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;	
};

#endif
