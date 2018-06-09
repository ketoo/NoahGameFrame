// -------------------------------------------------------------------------
//    @FileName         :    NFICommonRedisModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2012-12-15
//    @Module           :    NFICommonRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_COMMON_REDIS_MODULE_H
#define NFI_COMMON_REDIS_MODULE_H

#include "NFIModule.h"

class NFICommonRedisModule
    : public NFIModule
{
public:
	virtual std::string GetPropertyCacheKey(const NFGUID& self) = 0;
	virtual std::string GetRecordCacheKey(const NFGUID& self) = 0;

	virtual std::string GetAccountCacheKey(const std::string& strAccount) = 0;
	virtual std::string GetTileCacheKey(const int& nSceneID) = 0;

	virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& strClassName) = 0;
	virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& strClassName) = 0;

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyInfo(const NFGUID& self, const std::string& strClassName) = 0;
	virtual NF_SHARE_PTR<NFIRecordManager> GetRecordInfo(const NFGUID& self, const std::string& strClassName) = 0;

	virtual bool SavePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond = 0) = 0;
	virtual bool SaveRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond = 0) = 0;
};

#endif