// -------------------------------------------------------------------------
//    @FileName			:    NFCBigMapRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCBigMapRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_BIGMAP_REDIS_MODULE_H
#define NFC_BIGMAP_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"

class NFCBigMapRedisModule : public NFIBigMapRedisModule
{
public:

	NFCBigMapRedisModule(NFIPluginManager* p);
   
};


#endif
