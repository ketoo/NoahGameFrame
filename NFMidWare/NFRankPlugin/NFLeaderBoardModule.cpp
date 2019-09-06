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


#include "NFLeaderBoardModule.h"

bool NFLeaderBoardModule::Init()
{

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
    return true;
}

bool NFLeaderBoardModule::Shut()
{
    return true;
}

bool NFLeaderBoardModule::Execute()
{
    return true;
}

bool NFLeaderBoardModule::AfterInit()
{

    return true;
}

std::string NFLeaderBoardModule::MakeRanKey(const RANK_TYPE type, const int64_t nAreaID)
{
    if(nAreaID > 0)
    {
        return "Rank_" + to_string(type) + "_" + to_string(nAreaID);
    }

    return "Rank_" + to_string(type);
}

void NFLeaderBoardModule::AddValue(const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type, NFINT64 value)
{
    AddValue(0, self, type, value);

}

void NFLeaderBoardModule::SetValue(const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type, NFINT64 value)
{
    SetValue(0, self, type, value);
}

void NFLeaderBoardModule::SubValue(const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type, NFINT64 value)
{
    SubValue(0, self, type, value);
}

void NFLeaderBoardModule::RemoveValue(const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type)
{
    RemoveValue(0, self, type);
}

NFLeaderBoardModule::RankValue NFLeaderBoardModule::GetIndex(const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type)
{
    return GetIndex(0, self, type);
}

int NFLeaderBoardModule::RangeByIndex(const NFINT64 startIndex, const NFINT64 endIndex, const NFILeaderBoardModule::RANK_TYPE type, std::vector<NFILeaderBoardModule::RankValue>& vector)
{
    return RangeByIndex(0, startIndex, endIndex, type, vector);
}

int NFLeaderBoardModule::RangeByScore(const NFINT64 startScore, const NFINT64 endScore, const NFILeaderBoardModule::RANK_TYPE type, std::vector<NFILeaderBoardModule::RankValue>& vector)
{
    return RangeByScore(0, startScore, endScore, type, vector);
}

int NFLeaderBoardModule::GetRankListCount(const NFILeaderBoardModule::RANK_TYPE type)
{
    return GetRankListCount(0, type);
}

//for area
void NFLeaderBoardModule::AddValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
		double newValue;
        xNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), value, newValue);
    }
}

void NFLeaderBoardModule::SetValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        xNoSqlDriver->ZADD(self.ToString(), strRankKey, value);
    }
}

void NFLeaderBoardModule::SubValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
		double newValue;
		xNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), -value, newValue);
    }
}

void NFLeaderBoardModule::RemoveValue(const int64_t nAreaID, const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        xNoSqlDriver->ZREM(strRankKey, self.ToString());
    }
}

NFILeaderBoardModule::RankValue NFLeaderBoardModule::GetIndex(const int64_t nAreaID, const NFGUID& self, const NFILeaderBoardModule::RANK_TYPE type)
{
    NFLeaderBoardModule::RankValue xRankValue;
    std::string strRankKey = MakeRanKey(type, nAreaID);

    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        double value = 0;
        if (xNoSqlDriver->ZSCORE(strRankKey, self.ToString(), value))
        {
            xRankValue.id = self;
            xRankValue.score = value;

            xNoSqlDriver->ZRANK(strRankKey, self.ToString(), xRankValue.index);
        }
    }

    return xRankValue;
}

int NFLeaderBoardModule::RangeByIndex(const int64_t nAreaID, const NFINT64 startIndex, const NFINT64 endIndex, const RANK_TYPE type, std::vector<NFILeaderBoardModule::RankValue>& vector)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);

    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        std::vector<std::pair<std::string, double> > memberScoreVec;
        if (xNoSqlDriver->ZRANGE(strRankKey, startIndex, endIndex, memberScoreVec))
        {
            for (int i = 0; i < memberScoreVec.size(); ++i)
            {
                RankValue xRankValue;
                xRankValue.id.FromString(memberScoreVec[i].first);
                xRankValue.score = (int)memberScoreVec[i].second;
                xRankValue.index = i + startIndex;
                vector.push_back(xRankValue);
            }
        }
    }

    return vector.size();
}

int NFLeaderBoardModule::RangeByScore(const int64_t nAreaID, const NFINT64 startScore, const NFINT64 endScore, const RANK_TYPE type, std::vector<NFILeaderBoardModule::RankValue>& vector)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);

    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        std::vector<std::pair<std::string, double> > memberScoreVec;
        if (xNoSqlDriver->ZRANGEBYSCORE(strRankKey, startScore, endScore, memberScoreVec))
        {
            for (int i = 0; i < memberScoreVec.size(); ++i)
            {
                RankValue xRankValue;
                xRankValue.id.FromString(memberScoreVec[i].first);
                xRankValue.score = memberScoreVec[i].second;

                //xNoSqlDriver->ZRank(strRankKey, memberScoreVec[i].first, xRankValue.index);

                vector.push_back(xRankValue);
            }
        }
    }

    return vector.size();
}


int NFLeaderBoardModule::GetRankListCount(const int64_t nAreaID, const NFILeaderBoardModule::RANK_TYPE type)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);

    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        int nCount = 0;
        xNoSqlDriver->ZCARD(strRankKey, nCount);
        return nCount;
    }

    return 0;
}

