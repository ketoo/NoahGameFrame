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

#ifndef NF_PROXYSERVER_TO_GAME_MODULE_H
#define NF_PROXYSERVER_TO_GAME_MODULE_H

#include <string>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"


class NFProxyServerToGameModule : public NFIProxyServerToGameModule
{
public:

    NFProxyServerToGameModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    void OnSocketGSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void Register(NFINet* pNet);

    void OnAckEnterGame(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void LogServerInfo(const std::string& strServerInfo);

	void Transport(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
	NFINetClientModule* m_pNetClientModule;

};

#endif