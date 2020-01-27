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

#ifndef NF_HELLO_WORLD_H
#define NF_HELLO_WORLD_H

#include <iostream>

#include "NFComm/NFCore/NFObject.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIWSModule.h"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

/*
IN THIS PLUGIN:
YOU WILL KNOW HOW TO USE THE "NFIHttpServerModule" AND "NFIHttpClientModule" TO PROCESS WEB SERVICES AND ACCESS WEB RESOURCES
*/

class NFIHelloWorld5
	: public NFIModule
{

};

class NFHelloWorld5
    : public NFIHelloWorld5
{
public:
    NFHelloWorld5(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	bool OnCommandQuery(NF_SHARE_PTR<NFHttpRequest> req);

	NFWebStatus OnFilter(NF_SHARE_PTR<NFHttpRequest> req);

	int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

	void OnGetCallBack(const NFGUID id, const int state_code, const std::string& strRespData);
	void OnPostCallBack(const NFGUID id, const int state_code, const std::string& strRespData, const std::string& strMemoData);

    void OnWebSocketTestProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnTCPEvent(const NFSOCK fd, const NF_NET_EVENT event, NFINet* pNet);

	void OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:

	NFIClassModule* m_pLogicClassModule;
	NFIHttpServerModule* m_pHttpNetModule;
	NFIHttpClientModule* m_pHttpClientModule;
	NFIWSModule* m_pWSModule;
	NFIElementModule* m_pElementModule;
	NFIScheduleModule* m_pScheduleModule;
	NFINetModule* m_pNetModule;
};

#endif
