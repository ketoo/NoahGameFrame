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
#ifndef NF_WORLDNET_CLIENT_MODULE_H
#define NF_WORLDNET_CLIENT_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFISecurityModule.h"

class NFWorldToMasterModule
	: public NFIWorldToMasterModule
{
public:
	NFWorldToMasterModule(NFIPluginManager* p)
	{
		pPluginManager = p;
		mLastReportTime = 0;
        m_bIsExecute = true;
	}

	virtual bool Init();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

protected:

	void OnSocketMSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	
	void OnClientDisconnect(const NFSOCK nAddress);
	
	void OnClientConnected(const NFSOCK nAddress);

	virtual void LogServerInfo(const std::string& strServerInfo);


	void Register(NFINet* pNet);
	void ServerReport();
	void RefreshWorldInfo();
	void OnServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void OnSelectServerProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnKickClientProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void InvalidMessage(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
private:
	NFINT64 mLastReportTime;

	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIClassModule* m_pClassModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFINetModule* m_pNetModule;
	NFISecurityModule* m_pSecurityModule;
};

#endif