// -------------------------------------------------------------------------
//    @FileName         :    NFIGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIGuildRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GUILD_REDIS_MODULE_H
#define NFI_GUILD_REDIS_MODULE_H

#include "NFIModule.h"

class NFIGuildRedisModule
    : public NFIModule
{

public:
    virtual NF_SHARE_PTR<NFIPropertyManager> GetGuildCachePropertyInfo(const NFGUID& xGuid) = 0;
    virtual NF_SHARE_PTR<NFIRecordManager> GetGuildCacheRecordManager(const NFGUID& xGuid) = 0;

	//virtual bool GetGuildCachePropertyInfo(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIPropertyManager>>& xPMList) = 0;
	//virtual bool GetGuildCacheRecordManager(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIRecordManager>>& xRMList) = 0;

    virtual bool SetGuildCachePropertyInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager) = 0;
    virtual bool SetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager> pRecordManager) = 0;
};

#endif