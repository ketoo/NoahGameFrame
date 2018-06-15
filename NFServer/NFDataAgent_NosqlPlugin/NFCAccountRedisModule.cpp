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


bool NFCAccountRedisModule::driverOk()
{
    NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (xNoSqlDriver)
    {
        return true;
    }

    return false;

}

int NFCAccountRedisModule::VerifyAccount(const std::string& strAccount, const std::string& strPwd)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
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
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSet(strAccountKey, "Password", strPwd);
	}
	return false;
}

bool NFCAccountRedisModule::ExistAccount(const std::string & strAccount)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->Exists(strAccountKey);
	}

	return false;
}

bool NFCAccountRedisModule::ExistRoleName(const std::string & strRoleName)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->Exists(strRoleName);
	}

	return false;
}

bool NFCAccountRedisModule::CreateRole(const std::string & strAccount, const std::string & strRoleName, const NFGUID & id)
{
	const std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->Exists(strAccountKey) && !xNoSqlDriver->Exists(strRoleName))
		{
			xNoSqlDriver->HSet(strAccountKey, NFrame::Player::Name(), strRoleName);
			xNoSqlDriver->HSet(strAccountKey, NFrame::Player::ID(), id.ToString());

            NF_SHARE_PTR<NFINoSqlDriver> xRoleNameNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
            if (xRoleNameNoSqlDriver)
            {
                xRoleNameNoSqlDriver->HSet(strRoleName, NFrame::Player::ID(), id.ToString());
            }
			
			std::vector<std::string> vKey;
			std::vector<std::string> vValue;
			vKey.push_back(NFrame::Player::Account());
			vValue.push_back(strAccount);

			xNoSqlDriver->HMSet(m_pCommonRedisModule->GetPropertyCacheKey(id), vKey, vValue);
			//xNoSqlDriver->HMSet(m_pCommonRedisModule->GetRecordCacheKey(id), vKey, vValue);

			return true;
		}
	}

	return false;
}

bool NFCAccountRedisModule::GetRoleInfo(const std::string & strAccount, std::string & strRoleName, NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->Exists(strAccountKey))
		{
			std::string strID;
			bool bRoleNameRet = xNoSqlDriver->HGet(strAccountKey, NFrame::Player::Name(), strRoleName);
			bool bRoleIDRet = xNoSqlDriver->HGet(strAccountKey, NFrame::Player::ID(), strID);
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
