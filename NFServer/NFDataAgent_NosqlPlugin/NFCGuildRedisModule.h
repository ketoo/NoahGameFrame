// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCGuildRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_REDIS_MODULE_H
#define NFC_GUILD_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFCGuildRedisModule : public NFIGuildRedisModule
{
public:

	NFCGuildRedisModule(NFIPluginManager* p);

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

public:
    virtual NF_SHARE_PTR<NFIPropertyManager> GetGuildCachePropertyInfo(const NFGUID& xGuid);
    virtual NF_SHARE_PTR<NFIRecordManager> GetGuildCacheRecordManager(const NFGUID& xGuid);

    virtual bool SetGuildCachePropertyInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager);
    virtual bool SetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager> pRecordManager);

	//virtual bool GetGuildCachePropertyInfo(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIPropertyManager>>& xPMList);
	//virtual bool GetGuildCacheRecordManager(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIRecordManager>>& xRMList);

protected:
    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

protected:
    NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
    NFICommonRedisModule* m_pCommonRedisModule;
    NFIKernelModule* m_pKernelModule;

};


#endif
