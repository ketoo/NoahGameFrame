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


#include "NFRankModule.h"

bool NFRankModule::Init()
{

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
    return true;
}

bool NFRankModule::Shut()
{
    return true;
}

bool NFRankModule::Execute()
{
    return true;
}

bool NFRankModule::AfterInit()
{

    return true;
}

std::string NFRankModule::MakeRanKey(const RANK_TYPE type, const int64_t nAreaID)
{
    if(nAreaID > 0)
    {
        return "Rank_" + to_string(type) + "_" + to_string(nAreaID);
    }

    return "Rank_" + to_string(type);
}

void NFRankModule::AddValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value)
{
    AddValue(0, self, type, value);

}

void NFRankModule::SetValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value)
{
    SetValue(0, self, type, value);
}

void NFRankModule::SubValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value)
{
    SubValue(0, self, type, value);
}

void NFRankModule::RemoveValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type)
{
    RemoveValue(0, self, type);
}

NFRankModule::RankValue NFRankModule::GetIndex(const NFGUID& self, const NFIRankModule::RANK_TYPE type)
{
    return GetIndex(0, self, type);
}

int NFRankModule::RangeByIndex(const NFINT64 startIndex, const NFINT64 endIndex, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector)
{
    return RangeByIndex(0, startIndex, endIndex, type, vector);
}

int NFRankModule::RangeByScore(const NFINT64 startScore, const NFINT64 endScore, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector)
{
    return RangeByScore(0, startScore, endScore, type, vector);
}

int NFRankModule::GetRankListCount(const NFIRankModule::RANK_TYPE type)
{
    return GetRankListCount(0, type);
}

//for area
void NFRankModule::AddValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
		double newValue;
        xNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), value, newValue);
    }
}

void NFRankModule::SetValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        xNoSqlDriver->ZADD(self.ToString(), strRankKey, value);
    }
}

void NFRankModule::SubValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
		double newValue;
		xNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), -value, newValue);
    }
}

void NFRankModule::RemoveValue(const int64_t nAreaID, const NFGUID& self, const NFIRankModule::RANK_TYPE type)
{
    std::string strRankKey = MakeRanKey(type, nAreaID);
    NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriver(strRankKey);
    if (xNoSqlDriver)
    {
        xNoSqlDriver->ZREM(strRankKey, self.ToString());
    }
}

NFIRankModule::RankValue NFRankModule::GetIndex(const int64_t nAreaID, const NFGUID& self, const NFIRankModule::RANK_TYPE type)
{
    NFRankModule::RankValue xRankValue;
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

int NFRankModule::RangeByIndex(const int64_t nAreaID, const NFINT64 startIndex, const NFINT64 endIndex, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector)
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

int NFRankModule::RangeByScore(const int64_t nAreaID, const NFINT64 startScore, const NFINT64 endScore, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector)
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


int NFRankModule::GetRankListCount(const int64_t nAreaID, const NFIRankModule::RANK_TYPE type)
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

