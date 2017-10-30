// -------------------------------------------------------------------------
//    @FileName			:    NFCRankRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCRankRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_COMMON_REDIS_MODULE_H
#define NFC_COMMON_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFCCommonRedisModule : public NFICommonRedisModule
{
public:
    NFCCommonRedisModule(NFIPluginManager* p);

    virtual bool AfterInit();

public:
    virtual std::string GetPropertyCacheKey(const NFGUID& self);
	virtual std::string GetRecordCacheKey(const NFGUID& self);

	virtual std::string GetAccountCacheKey(const std::string& strAccount);
	virtual std::string GetTileCacheKey(const int& nSceneID);

    virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& strClassName);

    virtual NF_SHARE_PTR<NFIPropertyManager> GetCachePropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);
    virtual NF_SHARE_PTR<NFIRecordManager> GetCacheRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);

	//support hmset
    virtual bool SaveCachePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond = 0);
    virtual bool SaveCacheRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond = 0);

	//support hmset
    virtual bool ConvertPBToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager);
    virtual bool ConvertPBToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager);

	//support hmset
	virtual bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);
	virtual bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList);

protected:
	virtual bool ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase* pRecordData);
	virtual bool ConvertPBToRecord(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase* pRecordData);

protected:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;
};


#endif
