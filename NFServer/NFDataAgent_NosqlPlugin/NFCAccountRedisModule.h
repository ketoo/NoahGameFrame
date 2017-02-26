// -------------------------------------------------------------------------
//    @FileName			:    NFCAccountRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-07
//    @Module           :    NFCAccountRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_ACCOUNT_REDIS_MODULE_H
#define NFC_ACCOUNT_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIAccountRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFCAccountRedisModule : public NFIAccountRedisModule
{
public:

	NFCAccountRedisModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual int VerifyAccount(const std::string& strAccount, const std::string& strPwd);
	virtual bool AddAccount(const std::string& strAccount, const std::string& strPwd);
	virtual bool ExistAccount(const std::string& strAccount);

	virtual bool ExistRoleName(const std::string& strRoleName);
	virtual bool CreateRole(const std::string& strAccount, const std::string& strRoleName, const NFGUID& id);
	virtual bool GetRoleInfo(const std::string& strAccount, std::string& strRoleName, NFGUID& id);
protected:

private:
	NFIClassModule* m_pLogicClassModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFICommonRedisModule* m_pCommonRedisModule;
	NFIKernelModule* m_pKernelModule;
};


#endif
