// -------------------------------------------------------------------------
//    @FileName         :    NFIGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIGuildRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GUILD_REDIS_MODULE_H
#define NFI_GUILD_REDIS_MODULE_H

#include "NFILogicModule.h"

class NFIGuildRedisModule
    : public NFILogicModule
{

public:
    virtual NF_SHARE_PTR<NFIPropertyManager> GetGuildCachePropertyInfo(const NFGUID& xGuid) = 0;
    virtual NF_SHARE_PTR<NFIRecordManager> GetGuildCacheRecordManager(const NFGUID& xGuid) = 0;

    virtual bool SetGuildCachePropertyInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager) = 0;
    virtual bool SetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager>& pRecordManager) = 0;
};

#endif