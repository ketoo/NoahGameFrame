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

class NFCCommonRedisModule : public NFICommonRedisModule
{
public:

    NFCCommonRedisModule(NFIPluginManager* p);

public:
    virtual bool AfterInit();

public:
    virtual std::string GetPropertyCacheKey(const std::string& strClassName);
    virtual std::string GetRecordCacheKey(const std::string& strClassName);

    virtual NF_SHARE_PTR<NFIPropertyManager> NewPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> NewRecordManager(const std::string& strClassName);

    virtual NF_SHARE_PTR<NFIPropertyManager> GetCachePropertyInfo(const NFGUID& self, const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> GetCacheRecordInfo(const NFGUID& self, const std::string& strClassName);

    virtual bool SetCachePropertyInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager);
    virtual bool SetCacheRecordInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIRecordManager> pRecordManager);

    virtual bool RemoveCachePropertyInfo(const NFGUID& self, const std::string& strClassName);
    virtual bool RemoveCacheRecordInfo(const NFGUID& self, const std::string& strClassName);

    virtual bool ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& xMsg, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager);
    virtual bool ConvertPBToRecordManager(const NFMsg::ObjectRecordList& xMsg, NF_SHARE_PTR<NFIRecordManager>& pRecordManager);

    virtual bool ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager, NFMsg::ObjectPropertyList& xMsg, const bool bCheckCache = true);
    virtual bool ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, NFMsg::ObjectRecordList& xMsg, const bool bCheckCache = true);

protected:
    NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;
};


#endif
