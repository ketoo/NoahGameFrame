/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#ifndef NFC_HOME_MODULE_H
#define NFC_HOME_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICreateRoleModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIHomeModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIRankModule.h"

class NFCHomeModule
	: public NFIHomeModule
{
public:
	NFCHomeModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCHomeModule() {};

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();


	virtual void Relive(const NFGUID& self);

protected:

	void OnRequireReliveProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFIScheduleModule* m_pScheduleModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFILogModule* m_pLogModule;
	NFIRankModule* m_pRankModule;
};

#endif