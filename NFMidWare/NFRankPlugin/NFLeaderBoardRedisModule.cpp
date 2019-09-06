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

#include "NFLeaderBoardRedisModule.h"

NFLeaderBoardRedisModule::NFLeaderBoardRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFLeaderBoardRedisModule::Init()
{
	return true;
}

bool NFLeaderBoardRedisModule::Shut()
{
	return true;
}

bool NFLeaderBoardRedisModule::Execute()
{
	return true;
}

bool NFLeaderBoardRedisModule::AfterInit()
{
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFLeaderBoardRedisModule::SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZADD(strRankKey, self.ToString(), value);
}

bool NFLeaderBoardRedisModule::IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	double newValue;
	return pNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), value, newValue);
}

bool NFLeaderBoardRedisModule::RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID & self)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREM(strRankKey, self.ToString());
}

bool NFLeaderBoardRedisModule::RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREMRANGEBYRANK(strRankKey, nStart, nStop);
}

bool NFLeaderBoardRedisModule::RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREMRANGEBYSCORE(strRankKey, nMin, nMax);
}

bool NFLeaderBoardRedisModule::GetRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, double& dwValue)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZSCORE(strRankKey, self.ToString(), dwValue);
}

bool NFLeaderBoardRedisModule::GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREVRANGE(strRankKey, nStart, nStop, memberScoreVec);
}

bool NFLeaderBoardRedisModule::GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRANGEBYSCORE(strRankKey, nMin, nMax, memberScoreVec);
}

int NFLeaderBoardRedisModule::GetRankMemberCount(const NFMsg::ERankType eRankType)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCARD(strRankKey, nCount);

	return nCount;
}

int NFLeaderBoardRedisModule::GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCOUNT(strRankKey, nMin, nMax, nCount);

	return nCount;
}

std::string NFLeaderBoardRedisModule::MakeRedisKey(const NFMsg::ERankType eRankType)
{
	std::string szKey = "Rank_" + ERankType_Name(eRankType);
	
	return szKey;
}
