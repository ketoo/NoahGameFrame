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

#ifndef NF_HTTP_SERVER_MODULE_H
#define NF_HTTP_SERVER_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFCore/NFPerformance.hpp"
#include "NFComm/NFNetPlugin/NFIHttpServer.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFHttpServerModule
        : public NFIHttpServerModule
{

public:
    NFHttpServerModule(NFIPluginManager* p);

    virtual ~NFHttpServerModule();

public:

    virtual int InitServer(const unsigned short nPort);

    virtual bool Execute();

    virtual bool ResponseMsg(NF_SHARE_PTR<NFHttpRequest> req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK");

private:
	virtual bool OnReceiveNetPack(NF_SHARE_PTR<NFHttpRequest> req);
	virtual NFWebStatus OnFilterPack(NF_SHARE_PTR<NFHttpRequest> req);

	virtual bool AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR_PTR& cb);
	virtual bool AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR_PTR& cb);

private:
	NFIHttpServer* m_pHttpServer;
	NFILogModule* m_pLogModule;

	NFMapEx<NFHttpType, std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>> mMsgCBMap;
	std::map<std::string, HTTP_FILTER_FUNCTOR_PTR> mMsgFliterMap;

    HTTP_RECEIVE_FUNCTOR_PTR mComMsgCBList;
};

#endif
