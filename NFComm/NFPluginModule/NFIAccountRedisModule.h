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
	virtual bool VerifyAccount(const std::string& strAccount, const std::string& strPwd) = 0;
	virtual bool AddAccount(const std::string& strAccount, const std::string& strPwd) = 0;
	virtual bool ExistAccount(const std::string& strAccount) = 0;
};

#endif