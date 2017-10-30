// -------------------------------------------------------------------------
//    @FileName			:    NFCPVPRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPVPRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PVP_REDIS_MODULE_H
#define NFC_PVP_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPVPRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"

class NFCPVPRedisModule : public NFIPVPRedisModule
{
public:
	NFCPVPRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

protected:
	int OnObjectPlayerEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var);
	bool SavePlayerData(const NFGUID& self);

private:
	NFINoSqlModule* m_pNoSqlModule;
	NFIKernelModule* m_pKernelModule;
	NFICommonRedisModule* m_pCommonRedisModule;
};


#endif
