// -------------------------------------------------------------------------
//    @FileName         :    NFIAccountRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-07
//    @Module           :    NFIAccountRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_ACCOUNT_REDIS_MODULE_H
#define NFI_ACCOUNT_REDIS_MODULE_H

#include "NFIModule.h"

class NFIAccountRedisModule
    : public NFIModule
{

public:
	virtual int VerifyAccount(const std::string& strAccount, const std::string& strPwd) = 0;
	virtual bool AddAccount(const std::string& strAccount, const std::string& strPwd) = 0;
	virtual bool ExistAccount(const std::string& strAccount) = 0;

	virtual bool ExistRoleName(const std::string& strRoleName) = 0;
	virtual bool CreateRole(const std::string& strAccount, const std::string& strRoleName, const NFGUID& id) = 0;
	virtual bool GetRoleInfo(const std::string& strAccount, std::string& strRoleName, NFGUID& id) = 0;
};

#endif