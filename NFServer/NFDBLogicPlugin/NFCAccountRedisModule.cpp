// -------------------------------------------------------------------------
//    @FileName			:    NFCAccountRedisModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-07
//    @Module           :    NFCAccountRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCAccountRedisModule.h"

bool NFCAccountRedisModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();

	return true;
}

bool NFCAccountRedisModule::Shut()
{
	return true;
}

bool NFCAccountRedisModule::Execute()
{
	return true;
}

bool NFCAccountRedisModule::AfterInit()
{

	return true;
}

int NFCAccountRedisModule::VerifyAccount(const std::string& strAccount, const std::string& strPwd)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		std::string strPassword;
		if (xNoSqlDriver->HGET(strAccountKey, "Password", strPassword) && strPassword == strPwd)
		{
			return 0;
		}
	}

	return 1;
}

bool NFCAccountRedisModule::AddAccount(const std::string & strAccount, const std::string & strPwd)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSET(strAccountKey, "Password", strPwd);
	}
	return false;
}

bool NFCAccountRedisModule::ExistAccount(const std::string & strAccount)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->EXISTS(strAccountKey);
	}

	return false;
}
