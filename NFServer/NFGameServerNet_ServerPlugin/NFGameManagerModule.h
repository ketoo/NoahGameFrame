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

#ifndef NFGAMEDEMO_NFGAMEMANAGERMODULE_H
#define NFGAMEDEMO_NFGAMEMANAGERMODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"

class NFIGameManagerModule : public NFIModule
{
public:

};

class NFGameManagerModule
		: public NFIGameManagerModule
{
public:
	NFGameManagerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

protected:
	void OnClientPropertyIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientPropertyFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientPropertyStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientPropertyObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientPropertyVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientPropertyVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void OnClientAddRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRemoveRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientSwapRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnClientRecordVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

protected:
	//get GM Level and what you want to modify
	std::shared_ptr<NFIProperty> CalProperty(NF_SHARE_PTR<NFIObject> pObject, const NFGUID objectID, const std::string& propertyName, int& gmLevel);
	std::shared_ptr<NFIRecord> CalRecord(NF_SHARE_PTR<NFIObject> pObject, const NFGUID objectID, const std::string& recordName, int& gmLevel);

private:

	//////////////////////////////////////////////////////////////////////////
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneModule* m_pSceneModule;
	NFINetClientModule* m_pNetClientModule;
	NFIScheduleModule* m_pScheduleModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif