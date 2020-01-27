/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NFI_HTTP_NET_MODULE_H
#define NFI_HTTP_NET_MODULE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFNetPlugin/NFIHttpServer.h"

class NFIHttpServerModule
	: public NFIModule
{
public:
	virtual ~NFIHttpServerModule() {};

	// register msg callback
	template<typename BaseType>
	bool AddRequestHandler(const std::string& strPath, const NFHttpType eRequestType, BaseType* pBase, bool (BaseType::*handleRecieve)(NF_SHARE_PTR<NFHttpRequest> req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		HTTP_RECEIVE_FUNCTOR_PTR functorPtr(new HTTP_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(strPath, eRequestType, functorPtr);
	}

	template<typename BaseType>
	bool AddNetFilter(const std::string& strPath, BaseType* pBase, NFWebStatus(BaseType::*handleFilter)(NF_SHARE_PTR<NFHttpRequest> req))
	{
		HTTP_FILTER_FUNCTOR functor = std::bind(handleFilter, pBase, std::placeholders::_1);
		HTTP_FILTER_FUNCTOR_PTR functorPtr(new HTTP_FILTER_FUNCTOR(functor));

		return AddFilterCB(strPath, functorPtr);
	}
public:
	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool ResponseMsg(NF_SHARE_PTR<NFHttpRequest> req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK") = 0;

private:
	virtual bool AddMsgCB(const std::string& strPath, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR_PTR& cb) = 0;
	virtual bool AddFilterCB(const std::string& strPath, const HTTP_FILTER_FUNCTOR_PTR& cb) = 0;

};
#endif
