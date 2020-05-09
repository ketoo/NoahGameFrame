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

#ifndef NF_CREATEROLE_MODULE_H
#define NF_CREATEROLE_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToDBModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIDataTailModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFICreateRoleModule : public NFIModule
{

};

class NFCreateRoleModule
    : public NFICreateRoleModule
{
public:
    NFCreateRoleModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCreateRoleModule() {};

    virtual bool Init();
    virtual bool Shut();
	virtual bool ReadyExecute();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	void OnRequireRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnResponseRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
	void OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
	void OnDBLoadRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	int OnObjectPlayerEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var);


private:

	bool ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase* pRecordData);
	bool ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord);
	bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecord, NFMsg::ObjectRecordList* pRecordData);
	bool ConvertPBToRecordManager(const NFMsg::ObjectRecordList& pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord);

	bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pProps, NFMsg::ObjectPropertyList* pPropertyData);
	bool ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps);


	void AttachData(const NFGUID& self);
	void SaveData(const NFGUID& self);

	int SaveDataOnTime(const NFGUID& self, const std::string& name, const float fIntervalTime, const int nCount);

private:

	std::map<NFGUID, NFMsg::RoleDataPack> mxObjectDataCache;

private:
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFISceneModule* m_pSceneModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIGameServerToDBModule* m_pGameToDBModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFINetClientModule* m_pNetClientModule;
	NFIScheduleModule* m_pScheduleModule;
	NFIDataTailModule* m_pDataTailModule;
	NFIEventModule* m_pEventModule;
};

#endif