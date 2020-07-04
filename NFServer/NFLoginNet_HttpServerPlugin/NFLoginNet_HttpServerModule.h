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

#ifndef NF_LOGIN_NET_HTTP_MODULE_H
#define NF_LOGIN_NET_HTTP_MODULE_H

#include <map>
#include <iostream>

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILoginToMasterModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"

class NFILoginNet_HttpServerModule : public NFIModule
{

};

class NFLoginNet_HttpServerModule
	: public NFILoginNet_HttpServerModule
{
public:
	NFLoginNet_HttpServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
        m_bIsExecute = true;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute();

protected:
	bool OnLogin(NF_SHARE_PTR<NFHttpRequest> req);
	bool OnWorldView(NF_SHARE_PTR<NFHttpRequest> req);
	bool OnWorldSelect(NF_SHARE_PTR<NFHttpRequest>req);

	bool OnCommonQuery(NF_SHARE_PTR<NFHttpRequest> req);

	NFWebStatus OnFilter(NF_SHARE_PTR<NFHttpRequest> req);

	std::string GetUserID(NF_SHARE_PTR<NFHttpRequest> req);
	std::string GetUserJWT(NF_SHARE_PTR<NFHttpRequest> req);
	bool CheckUserJWT(const std::string & user, const std::string & jwt);


	std::map<std::string, std::string> mToken;

private:
	NFINetClientModule* m_pNetClientModule;
	NFIKernelModule* m_pKernelModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFILoginNet_ServerModule* m_pLoginServerModule;
	NFILoginToMasterModule* m_pLoginToMasterModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
};

#endif