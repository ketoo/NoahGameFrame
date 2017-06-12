// -------------------------------------------------------------------------
//    @FileName			:	NFCGuildRedisModule.h
//    @Author           :	LvSheng.Huang
//    @Date             :	2017-06-04
//    @Module           :	NFCGuildRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_REDIS_MODULE_H
#define NFC_GUILD_REDIS_MODULE_H

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"

class NFCGuildRedisModule
	: public NFIGuildRedisModule
{
public:
	NFCGuildRedisModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
};

#endif