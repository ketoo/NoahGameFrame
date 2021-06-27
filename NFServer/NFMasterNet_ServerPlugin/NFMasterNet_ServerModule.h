/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#ifndef NF_MASTERNET_SERVER_MODULE_H
#define NF_MASTERNET_SERVER_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFMasterNet_ServerModule
    : public NFIMasterNet_ServerModule
{
public:
    NFMasterNet_ServerModule(NFIPluginManager* p)
    {
		executableModule = true;
		pPluginManager = p;
        mnLastLogTime = pPluginManager->GetNowTime();
    }
	virtual ~NFMasterNet_ServerModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool AfterInit();
    virtual bool Execute();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

	virtual std::string GetServersStatus();

protected:

    void OnSocketEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
    void OnClientDisconnect(const NFSOCK nAddress);
    void OnClientConnected(const NFSOCK nAddress);

protected:
    void OnWorldRegisteredProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
    void OnWorldUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
    void OnRefreshWorldInfoProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);

    //////////////////////////////////////////////////////////////////////////
    void OnLoginRegisteredProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
    void OnLoginUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
    void OnRefreshLoginInfoProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);

    void OnSelectWorldProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
    void OnSelectServerResultProcess(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);

	void OnServerReport(const NFSOCK nFd, const int msgId, const std::string_view& msg);

    //////////////////////////////////////////////////////////////////////////

    void SynWorldToLoginAndWorld();
    void LogGameServer();

	void OnHeartBeat(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);
	void InvalidMessage(const NFSOCK sockIndex, const int msgID, const std::string_view& msg);

private:

    NFINT64 mnLastLogTime;

    //serverid,data
	NFMapEx<int, ServerData> mMasterMap;
	NFMapEx<int, ServerData> mLoginMap;
    NFMapEx<int, ServerData> mWorldMap;
	NFMapEx<int, ServerData> mProxyMap;
	NFMapEx<int, ServerData> mGameMap;


    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
};

#endif