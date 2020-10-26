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
    virtual std::string GetPropertyCacheKey(const std::string& self);
	virtual std::string GetRecordCacheKey(const std::string& self);
	virtual std::string GetFriendCacheKey(const std::string& self);
	virtual std::string GetFriendInviteCacheKey(const std::string& self);
	virtual std::string GetBlockCacheKey(const std::string& self);

	virtual std::string GetTeamCacheKey(const std::string& self);
	virtual std::string GetTeamInviteCacheKey(const std::string& self);

	virtual std::string GetAccountCacheKey(const std::string& account);
	virtual std::string GetTileCacheKey(const int& sceneID);
	virtual std::string GetSceneCacheKey(const int& sceneID);
	virtual std::string GetCellCacheKey(const std::string& strCellID);

    virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& className);
    virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& className);

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyInfo(const std::string& self, const std::string& className, const bool cache, const bool save, NF_SHARE_PTR<NFIPropertyManager> propertyManager = nullptr);
	virtual NF_SHARE_PTR<NFIRecordManager> GetRecordInfo(const std::string& self, const std::string& className, const bool cache, const bool save, NF_SHARE_PTR<NFIRecordManager> recordManager = nullptr);
	virtual bool GetRecordInfo(const std::string& self, const std::string& className, NFMsg::ObjectRecordList* pRecordData, const bool cache, const bool save);

	//support hmset
	virtual bool SavePropertyInfo(const std::string& self, const std::string& propertyName, const std::string& propertyValue);

	virtual bool SavePropertyInfo(const std::string& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const bool cache, const bool save, const int nExpireSecond = -1);
	virtual bool SaveRecordInfo(const std::string& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const bool cache, const bool save, const int nExpireSecond = -1);
	virtual bool SaveRecordInfo(const std::string& self, const NFMsg::ObjectRecordList& xRecordData, const int nExpireSecond = -1);
	virtual bool GetPropertyList(const std::string& self, const std::vector<std::string>& fields, std::vector<std::string>& values);
	virtual bool GetPropertyList(const std::string& self, std::vector<std::pair<std::string, std::string>>& values);

	virtual NFINT64 GetPropertyInt(const std::string& self, const std::string& propertyName);
	virtual int GetPropertyInt32(const std::string& self, const std::string& propertyName);
	virtual double GetPropertyFloat(const std::string& self, const std::string& propertyName);
	virtual std::string GetPropertyString(const std::string& self, const std::string& propertyName);
	virtual NFGUID GetPropertyObject(const std::string& self, const std::string& propertyName);
	virtual NFVector2 GetPropertyVector2(const std::string& self, const std::string& propertyName);
	virtual NFVector3 GetPropertyVector3(const std::string& self, const std::string& propertyName);


	////
	static bool ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pRecordData);
	static bool ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord);
	static bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager> pRecord, NFMsg::ObjectRecordList* pRecordData, const bool cache, const bool save);
	static bool ConvertPBToRecordManager(const NFMsg::ObjectRecordList& pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord);

	static bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager> pProps, NFMsg::ObjectPropertyList* pPropertyData, const bool cache, const bool save);
	static bool ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps);

protected:
	//support hmset
	virtual bool ConvertVectorToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const bool cache, const bool save);
	virtual bool ConvertVectorToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const bool cache, const bool save);

	//support hmset
	virtual bool ConvertPropertyManagerToVector(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save);
	virtual bool ConvertRecordManagerToVector(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save);

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyInfo(const std::string& self, const std::string& className, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save, NF_SHARE_PTR<NFIPropertyManager> propertyManager = nullptr);
	virtual NF_SHARE_PTR<NFIRecordManager> GetRecordInfo(const std::string& self, const std::string& className, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save, NF_SHARE_PTR<NFIRecordManager> recordManager = nullptr);

protected:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;
};


#endif
