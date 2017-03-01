// -------------------------------------------------------------------------
//    @FileName         :    NFIPlayerRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPlayerRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PLAYER_REDIS_MODULE_H
#define NFI_PLAYER_REDIS_MODULE_H

#include "NFIModule.h"

class NFIPlayerRedisModule
    : public NFIModule
{

public:
	virtual int64_t GetPlayerCacheGameID(const NFGUID& self) = 0;
	virtual int64_t GetPlayerCacheProxyID(const NFGUID& self) = 0;

	virtual bool GetPlayerCacheGameID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList) = 0;
	virtual bool GetPlayerCacheProxyID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList) = 0;

	virtual bool LoadPlayerData(const NFGUID& self) = 0;
	virtual int GetPlayerHomeSceneID(const NFGUID& self) = 0;

	virtual NF_SHARE_PTR<NFIPropertyManager> GetPlayerCacheProperty(const NFGUID& self) = 0;
	virtual NF_SHARE_PTR<NFIRecordManager> GetPlayerCacheRecord(const NFGUID& self) = 0;

	virtual bool SavePlayerDataToCache(const NFGUID& self) = 0;
	virtual bool SetPlayerCacheProperty(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager) = 0;
	virtual bool SetPlayerCacheRecord(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager) = 0;

	virtual bool SavePlayerTileToCache(const int nSceneID, const NFGUID& self, const std::string& strTileData) = 0;
	virtual bool GetPlayerTileRandomFromCache(const int nSceneID, std::string& strTileData) = 0;
	virtual bool GetPlayerTileFromCache(const int nSceneID, const NFGUID& self, std::string& strTileData) = 0;
};

#endif