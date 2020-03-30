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
#ifndef NF_PROXYSERVER_SERVER_MODULE_H
#define NF_PROXYSERVER_SERVER_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFISecurityModule.h"
#include "NFComm/NFPluginModule/NFIWSModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"

class NFProxyServerNet_ServerModule : public NFIProxyServerNet_ServerModule
{
public:
    NFProxyServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual int Transport(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID);

protected:

    void OnSocketClientEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void OnClientDisconnect(const NFSOCK nAddress);
    void OnClientConnected(const NFSOCK nAddress);

    void OnConnectKeyProcessWS(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnConnectKeyProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqServerListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSelectServerProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqCreateRoleProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqDelRoleProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqEnterGameServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////

	void OnOtherMessage(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
protected:

    NFMapEx<NFGUID, NFSOCK> mxClientIdent;
protected:
    NFINetClientModule* m_pNetClientModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
	NFINetModule* m_pNetModule;
    NFIWSModule* m_pWsModule;
	NFISecurityModule* m_pSecurityModule;
	NFIProxyServerToWorldModule* m_pProxyToWorldModule;
    NFIThreadPoolModule* m_pThreadPoolModule;
};

#endif