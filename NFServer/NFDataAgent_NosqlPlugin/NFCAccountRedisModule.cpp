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

bool NFCAccountRedisModule::ExistRoleName(const std::string & strRoleName)
{
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->EXISTS(strRoleName);
	}

	return false;
}

bool NFCAccountRedisModule::CreateRole(const std::string & strAccount, const std::string & strRoleName, const NFGUID & id)
{
	const std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strAccountKey) && !xNoSqlDriver->EXISTS(strRoleName))
		{
			std::vector<std::string> vecFields;
			std::vector<std::string> vecValues;

			vecFields.push_back(NFrame::Player::Name());
			vecFields.push_back(NFrame::Player::ID());

			vecValues.push_back(strRoleName);
			vecValues.push_back(id.ToString());

			xNoSqlDriver->HMSET(strAccountKey, vecFields, vecValues);

            NF_SHARE_PTR<NFIRedisClient> xRoleNameNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
            if (xRoleNameNoSqlDriver)
            {
                xRoleNameNoSqlDriver->HSET(strRoleName, NFrame::Player::ID(), id.ToString());
            }
			
			std::vector<std::string> vKey;
			std::vector<std::string> vValue;
			vKey.push_back(NFrame::Player::Account());
			vValue.push_back(strAccount);

			NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonRedisModule->NewPropertyManager(NFrame::Player::ThisName());
			if (xPropertyManager)
			{
				NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->First();
				while (xProperty)
				{
					if (xProperty->GetKey() == NFrame::Player::Account())
					{
						continue;
					}
					else if (xProperty->GetKey() == NFrame::Player::Level())
					{
						vKey.push_back(xProperty->GetKey());
						vValue.push_back("1");
					}
					else
					{
						switch (xProperty->GetType())
						{
						case NFDATA_TYPE::TDATA_INT:
							break;
						case NFDATA_TYPE::TDATA_FLOAT:
							break;
						case NFDATA_TYPE::TDATA_STRING:
							break;
						case NFDATA_TYPE::TDATA_OBJECT:
							break;
						case NFDATA_TYPE::TDATA_VECTOR2:
							break;
						case NFDATA_TYPE::TDATA_VECTOR3:
							break;
						default:
							break;
						}
					}

					xProperty = xPropertyManager->First();
				}
			}

			xNoSqlDriver->HMSET(m_pCommonRedisModule->GetPropertyCacheKey(id), vKey, vValue);
			//xNoSqlDriver->HMSet(m_pCommonRedisModule->GetRecordCacheKey(id), vKey, vValue);

			return true;
		}
	}

	return false;
}

bool NFCAccountRedisModule::GetRoleInfo(const std::string & strAccount, std::string & strRoleName, NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strAccountKey))
		{
			std::string strID;
			bool bRoleNameRet = xNoSqlDriver->HGET(strAccountKey, NFrame::Player::Name(), strRoleName);
			bool bRoleIDRet = xNoSqlDriver->HGET(strAccountKey, NFrame::Player::ID(), strID);
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
