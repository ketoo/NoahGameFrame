// -------------------------------------------------------------------------
//    @FileName			:    NFCMailRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCMailRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_MAIL_REDIS_MODULE_H
#define NFC_MAIL_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIMailRedisModule.h"

class NFCMailRedisModule : public NFIMailRedisModule
{
public:

	NFCMailRedisModule(NFIPluginManager* p);

};


#endif
