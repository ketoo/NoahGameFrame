// -------------------------------------------------------------------------
//    @FileName         :    NFIPlayerRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPlayerRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PLAYER_REDIS_MODULE_H
#define NFI_PLAYER_REDIS_MODULE_H

#include "NFILogicModule.h"

class NFIPlayerRedisModule
    : public NFILogicModule
{

public:
	virtual int64_t GetPlayerCacheGameID(const NFGUID& self) = 0;
	virtual int64_t GetPlayerCacheProxyID(const NFGUID& self) = 0;

	virtual bool GetPlayerCacheGameID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList) = 0;
	virtual bool GetPlayerCacheProxyID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList) = 0;

};

#endif