// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPlayerRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PLAYER_REDIS_MODULE_H
#define NFC_PLAYER_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"

class NFCPlayerRedisModule : public NFIPlayerRedisModule
{
public:

	NFCPlayerRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual int64_t GetPlayerCacheGameID(const NFGUID& self);
	virtual int64_t GetPlayerCacheProxyID(const NFGUID& self);

	virtual bool GetPlayerCacheGameID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList);
	virtual bool GetPlayerCacheProxyID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList);

protected:
	std::string GetOnlineGameServerKey();
	std::string GetOnlineProxyServerKey();


private:
	NFILogicClassModule* m_pLogicClassModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFICommonRedisModule* m_pCommonRedisModule;

};


#endif
