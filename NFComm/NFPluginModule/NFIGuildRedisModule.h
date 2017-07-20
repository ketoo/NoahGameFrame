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
	virtual bool ExistGuild(const std::string& strName) = 0;
	virtual bool CreateGuild(const NFGUID& self, const std::string& strGuildName, const NFGUID& xCreater) = 0;



};

#endif