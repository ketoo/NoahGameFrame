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

	virtual bool ExistGuild(const std::string& strName);
	virtual bool CreateGuild(const NFGUID& self, const std::string& strGuildName, const NFGUID& xCreater);

protected:
    NFIClassModule* m_pLogicClassModule;
    NFINoSqlModule* m_pNoSqlModule;
    NFICommonRedisModule* m_pCommonRedisModule;
    NFIKernelModule* m_pKernelModule;

};


#endif
