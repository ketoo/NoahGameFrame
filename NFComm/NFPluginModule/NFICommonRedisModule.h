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

    virtual NF_SHARE_PTR<NFIPropertyManager> GetCachePropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList) = 0;
    virtual NF_SHARE_PTR<NFIRecordManager> GetCacheRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList) = 0;
	
	virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& strClassName) = 0;
	virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& strClassName) = 0;

	virtual bool SaveCachePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager) = 0;
	virtual bool SaveCacheRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager) = 0;

	//support hset
	virtual bool ConvertPBToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager) = 0;
	virtual bool ConvertPBToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager>& pRecordManager) = 0;

	//support hmset
	virtual bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList) = 0;
	virtual bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList) = 0;

};

#endif