// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCGuildRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_REDIS_MODULE_H
#define NFC_GUILD_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"

class NFCGuildRedisModule : public NFIGuildRedisModule
{
public:

	NFCGuildRedisModule(NFIPluginManager* p);

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

public:
    virtual NF_SHARE_PTR<NFIPropertyManager> GetGuildCacheInfo(const NFGUID& xGuid);
    virtual NF_SHARE_PTR<NFIRecordManager> GetGuildCacheRecordManager(const NFGUID& xGuid);

    virtual bool SetGuildCacheInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager);
    virtual bool SetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager>& pRecordManager);

protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
    NFICommonRedisModule* m_pCommonRedisModule;

};


#endif
