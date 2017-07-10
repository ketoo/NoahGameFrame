// -------------------------------------------------------------------------
//    @FileName      	:   NFIHttpClientModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-07-07
//    @Module           :   NFIHttpClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_HTTP_CLIENT_MODULE_H
#define NFI_HTTP_CLIENT_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFNetPlugin/NFIHttpClient.h"

class NFIHttpClientModule
	: public NFIModule
{
public:
	virtual ~NFIHttpClientModule() {};

	template<typename BaseType>
	bool PerformGet(const std::string& strUri,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformGet(strUri,pd,std::string(),std::map<std::string, std::string>());
	}

	template<typename BaseType>
	bool PerformGet(const std::string& strUri, const std::string& strUserData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformGet(strUri, pd, std::string(), std::map<std::string, std::string>());
	}

	template<typename BaseType>
	bool PerformGet(const std::string& strUri, const std::string& strUserData,const std::map<std::string, std::string>& xHeaders,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformGet(strUri, pd, strUserData, xHeaders);
	}

	template<typename BaseType>
	bool PerformPost(const std::string& strUri,const std::string& strPostData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformPost(strUri, strPostData, pd, strUserData, std::map<std::string, std::string>());
	}


	template<typename BaseType>
	bool PerformPost(const std::string& strUri,const std::string& strPostData, const std::string& strUserData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformPost(strUri,strPostData, pd,strUserData, std::map<std::string, std::string>());
	}


	template<typename BaseType>
	bool PerformPost(const std::string& strUri,const std::string& strPostData, const std::string& strUserData,const std::map<std::string, std::string>& xHeaders,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformPost(strUri, strPostData, pd, strUserData, xHeaders);
	}

protected:
	virtual bool PerformGet(const std::string& strUri,
		HTTP_RESP_FUNCTOR_PTR pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0 ;

	virtual bool PerformPost(const std::string& strUri,
		const std::string& strPostData,
		HTTP_RESP_FUNCTOR_PTR pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0;
};

#endif
