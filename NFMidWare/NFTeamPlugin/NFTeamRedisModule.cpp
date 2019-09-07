/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFTeamRedisModule.h"

NFTeamRedisModule::NFTeamRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFTeamRedisModule::Init()
{
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFTeamRedisModule::Shut()
{
	return true;
}

bool NFTeamRedisModule::Execute()
{
	return true;
}

bool NFTeamRedisModule::AfterInit()
{

	return true;
}

bool NFTeamRedisModule::GetMemberList(const NFGUID & teamID, std::vector<MemberData>& teamMember)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strKey))
		{
			std::vector<string_pair> values;
			if (xNoSqlDriver->HGETALL(strKey, values))
			{
				for (auto member : values)
				{
					MemberData memberData;
					NFGUID id(member.first);
					const std::string& name = member.second;

					memberData.id = id;
					memberData.name = name;

					teamMember.push_back(memberData);
				}

				return true;
			}
		}
	}

	return false;
}

bool NFTeamRedisModule::GetMemberInvitationList(const NFGUID & teamID, std::vector<MemberData>& teamMember)
{
	std::string strKey = m_pCommonRedisModule->GetTeamInviteCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strKey))
		{
			std::vector<string_pair> values;
			if (xNoSqlDriver->HGETALL(strKey, values))
			{
				for (auto member : values)
				{
					MemberData memberData;
					NFGUID id(member.first);
					const std::string& name = member.second;

					memberData.id = id;
					memberData.name = name;

					teamMember.push_back(memberData);
				}

				return true;
			}
		}
	}

	return false;
}

bool NFTeamRedisModule::CreateTeam(const NFGUID& self, const std::string& name, const NFGUID& teamID)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HSET(strKey, self.ToString(), name))
		{
			//xNoSqlDriver->EXPIRE(strKey, 3600 * 24 * 30);
			return true;
		}
	}

	return false;
}

bool NFTeamRedisModule::DeleteTeam(const NFGUID & teamID)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HDEL(strKey, teamID.ToString()))
		{
			return true;
		}
	}

	return false;
}

bool NFTeamRedisModule::AddToTeam(const NFGUID & teamID, const NFGUID & other, const std::string& name)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strKey))
		{
			if (xNoSqlDriver->HSET(strKey, other.ToString(), name))
			{
				return true;
			}
		}
	}

	return false;
}

bool NFTeamRedisModule::LeaveFromTeam(const NFGUID & teamID, const NFGUID & other)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strKey))
		{
			if (xNoSqlDriver->HDEL(strKey, other.ToString()))
			{
				return true;
			}
		}
	}

	return false;
}

bool NFTeamRedisModule::IsTeamMember(const NFGUID & teamID, const NFGUID & other)
{
	std::string strKey = m_pCommonRedisModule->GetTeamCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HEXISTS(strKey, other.ToString());
	}

	return false;
}

bool NFTeamRedisModule::SendInvite(const NFGUID& teamID, const NFGUID& stranger, const std::string& name)
{
    std::string strKey = m_pCommonRedisModule->GetTeamInviteCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HSET(strKey, stranger.ToString(), name))
		{
			return true;
		}
	}

    return false;
}

bool NFTeamRedisModule::DeleteInvite(const NFGUID & teamID, const NFGUID & stranger)
{
	std::string strKey = m_pCommonRedisModule->GetTeamInviteCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HDEL(strKey, stranger.ToString()))
		{
			return true;
		}
	}

	return false;
}

bool NFTeamRedisModule::DeleteAllInvite(const NFGUID & teamID)
{
	std::string strKey = m_pCommonRedisModule->GetTeamInviteCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->DEL(strKey))
		{
			return true;
		}
	}

	return false;
}

bool NFTeamRedisModule::IsInvited(const NFGUID & teamID, const NFGUID & stranger)
{
	std::string strKey = m_pCommonRedisModule->GetTeamInviteCacheKey(teamID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HEXISTS(strKey, stranger.ToString());
	}

	return false;
}
