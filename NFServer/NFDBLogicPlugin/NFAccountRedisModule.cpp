/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFAccountRedisModule.h"

bool NFAccountRedisModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();

	return true;
}

bool NFAccountRedisModule::Shut()
{
	return true;
}

bool NFAccountRedisModule::Execute()
{
	return true;
}

bool NFAccountRedisModule::AfterInit()
{

	return true;
}

bool NFAccountRedisModule::VerifyAccount(const std::string& strAccount, const std::string& strPwd)
{
	if (strAccount.empty() || strPwd.empty())
	{
		return false;
	}

	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		std::string strPassword;
		if (xNoSqlDriver->HGET(strAccountKey, "Password", strPassword) && strPassword == strPwd)
		{
			return true;
		}
	}

	return false;
}

bool NFAccountRedisModule::AddAccount(const std::string & strAccount, const std::string & strPwd)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSET(strAccountKey, "Password", strPwd);
	}
	return false;
}

bool NFAccountRedisModule::ExistAccount(const std::string & strAccount)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->EXISTS(strAccountKey);
	}

	return false;
}
