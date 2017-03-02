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
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		std::string strPassword;
		if (xNoSqlDriver->HGet(strAccountKey, "Password", strPassword) && strPassword == strPwd)
		{
			return 0;
		}
	}

	return 1;
}

bool NFCAccountRedisModule::AddAccount(const std::string & strAccount, const std::string & strPwd)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSet(strAccountKey, "Password", strPwd);
	}
	return false;
}

bool NFCAccountRedisModule::ExistAccount(const std::string & strAccount)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->Exists(strAccountKey);
	}

	return false;
}

bool NFCAccountRedisModule::ExistRoleName(const std::string & strRoleName)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->Exists(strRoleName);
	}

	return false;
}

bool NFCAccountRedisModule::CreateRole(const std::string & strAccount, const std::string & strRoleName, const NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->Exists(strAccountKey) && !xNoSqlDriver->Exists(strRoleName))
		{
			xNoSqlDriver->HSet(strAccountKey, "Name", strRoleName);
			xNoSqlDriver->HSet(strAccountKey, "RoleID", id.ToString());

			xNoSqlDriver->HSet(strRoleName, "RoleID", id.ToString());

			return true;
		}
	}

	return false;
}

bool NFCAccountRedisModule::GetRoleInfo(const std::string & strAccount, std::string & strRoleName, NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->Exists(strAccountKey))
		{
			std::string strID;
			bool bRoleNameRet = xNoSqlDriver->HGet(strAccountKey, "Name", strRoleName);
			bool bRoleIDRet = xNoSqlDriver->HGet(strAccountKey, "RoleID", strID);
			if (bRoleNameRet && bRoleIDRet
				&& !strRoleName .empty() && !strID.empty())
			{
				return id.FromString(strID);
			}

			return false;
		}
	}

	return false;
}
