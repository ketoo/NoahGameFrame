/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
    bool DoGet ( const std::string& strUri,BaseType* pBase,
						void ( BaseType::*handleRecieve ) (const NFGUID id, const int state_code, const std::string& strRespData ) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) ) );
        return DoGet ( strUri, std::map<std::string, std::string>(), pd );
    }

	bool DoGet(const std::string& strUri, HTTP_RESP_FUNCTOR functor)
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(functor));
		return DoGet(strUri, std::map<std::string, std::string>(), pd);
	}

    template<typename BaseType>
    bool DoGet ( const std::string& strUri, const std::map<std::string, std::string>& xHeaders,
                      BaseType* pBase,
                      void ( BaseType::*handleRecieve ) (const NFGUID id, const int state_code, const std::string& strRespData) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) ) );
        return DoGet( strUri, xHeaders, pd );
    }

	bool DoGet(const std::string& strUri, const std::map<std::string, std::string>& xHeaders,
		HTTP_RESP_FUNCTOR functor)
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(functor));
		return DoGet(strUri, xHeaders, pd);
	}


    template<typename BaseType>
    bool DoPost ( const std::string& strUri, const std::string& strPostData,
                       BaseType* pBase,
                       void ( BaseType::*handleRecieve ) (const NFGUID id, const int state_code, const std::string& strRespData, const std::string& strMemo), const std::string& strMemo = "")
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) ) );
        return DoPost( strUri, std::map<std::string, std::string>(), strPostData, pd ,strMemo);
    }

	bool DoPost(const std::string& strUri, const std::string& strPostData,
		HTTP_RESP_FUNCTOR functor, const std::string& strMemo = "")
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(functor));
		return DoPost(strUri, std::map<std::string, std::string>(), strPostData, pd,strMemo);
	}

    template<typename BaseType>
    bool DoPost( const std::string& strUri, const std::string& strPostData, const std::map<std::string, std::string>& xHeaders,
                       BaseType* pBase,
                       void ( BaseType::*handleRecieve ) (const NFGUID id, const int state_code, const std::string& strRespData ), const std::string& strMemo="")
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) ) );
        return DoPost( strUri, xHeaders, strPostData, pd,strMemo );
    }

	bool DoPost(const std::string& strUri, const std::string& strPostData, const std::map<std::string, std::string>& xHeaders,
		HTTP_RESP_FUNCTOR functor, const std::string& strMemo = "")
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(functor));
		return DoPost(strUri, xHeaders, strPostData, pd,strMemo);
	}
	//actually, sync post method
	//NFWebStatus
	virtual int Post(const std::string& strUri, const std::string& strData, std::string& strResData) = 0;
	virtual int Post(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, const std::string& strData, std::string& strResData) = 0;
	
	//actually, sync get method
	//NFWebStatus
	virtual int Get(const std::string& strUri, std::string& strResData) = 0;
	virtual int Get(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, std::string& strResData) = 0;

protected:
    virtual bool DoGet( const std::string& strUrl,
                              const std::map<std::string, std::string>& xHeaders,
                              HTTP_RESP_FUNCTOR_PTR pCB) = 0 ;

    virtual bool DoPost( const std::string& strUrl,
                               const std::map<std::string, std::string>& xHeaders,
                               const std::string& strPostData,
                               HTTP_RESP_FUNCTOR_PTR pCB, const std::string& strMemo) = 0;
};

#endif
