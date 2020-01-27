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


#ifndef NF_COMMON_REDIS_MODULE_H
#define NF_COMMON_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFCommonRedisModule : public NFICommonRedisModule
{
public:
    NFCommonRedisModule(NFIPluginManager* p);

    virtual bool AfterInit();

public:
    virtual std::string GetPropertyCacheKey(const NFGUID& self);
	virtual std::string GetRecordCacheKey(const NFGUID& self);
	virtual std::string GetFriendCacheKey(const NFGUID& self);
	virtual std::string GetFriendInviteCacheKey(const NFGUID& self);
	virtual std::string GetBlockCacheKey(const NFGUID& self);

	virtual std::string GetTeamCacheKey(const NFGUID& self);
	virtual std::string GetTeamInviteCacheKey(const NFGUID& self);

	virtual std::string GetAccountCacheKey(const std::string& strAccount);
	virtual std::string GetTileCacheKey(const int& nSceneID);
	virtual std::string GetSceneCacheKey(const int& nSceneID);
	virtual std::string GetCellCacheKey(const std::string& strCellID);

    virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& strClassName);

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyInfo(const NFGUID& self, const std::string& strClassName);
	virtual NF_SHARE_PTR<NFIRecordManager> GetRecordInfo(const NFGUID& self, const std::string& strClassName);
	virtual bool GetRecordInfo(const NFGUID& self, const std::string& strClassName, NFMsg::ObjectRecordList* pRecordData);

	//support hmset
    virtual bool SavePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond = 0);
	virtual bool SaveRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond = 0);
	virtual bool SaveRecordInfo(const NFGUID& self, const NFMsg::ObjectRecordList& xRecordData, const int nExpireSecond = 0);


	////
	bool ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase* pRecordData);
	bool ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord);
	bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecord, NFMsg::ObjectRecordList* pRecordData);
	bool ConvertPBToRecordManager(const NFMsg::ObjectRecordList& pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord);

	bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pProps, NFMsg::ObjectPropertyList* pPropertyData);
	bool ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps);

protected:
	//support hmset
	virtual bool ConvertVectorToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager);
	virtual bool ConvertVectorToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager);

	//support hmset
	virtual bool ConvertPropertyManagerToVector(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);
	virtual bool ConvertRecordManagerToVector(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);
	virtual NF_SHARE_PTR<NFIRecordManager> GetRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);

protected:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;
};


#endif
