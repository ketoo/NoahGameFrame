// -------------------------------------------------------------------------
//    @FileName			:    NFCRankRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCRankRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCRankRedisModule.h"

NFCRankRedisModule::NFCRankRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCRankRedisModule::Init()
{
	return true;
}

bool NFCRankRedisModule::Shut()
{
	return true;
}

bool NFCRankRedisModule::Execute()
{
	return true;
}

bool NFCRankRedisModule::AfterInit()
{
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFCRankRedisModule::SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZAdd(strRankKey, value, self.ToString());
}

bool NFCRankRedisModule::IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZIncrBy(strRankKey, self.ToString(), value);
}

bool NFCRankRedisModule::RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID & self)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRem(strRankKey, self.ToString());
}

bool NFCRankRedisModule::RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRemRangeByRank(strRankKey, nStart, nStop);
}

bool NFCRankRedisModule::RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRemRangeByScore(strRankKey, nMin, nMax);
}

bool NFCRankRedisModule::GetRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, double& dwValue)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZScore(strRankKey, self.ToString(), dwValue);
}

bool NFCRankRedisModule::GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRevRange(strRankKey, nStart, nStop, memberScoreVec);
}

bool NFCRankRedisModule::GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRangeByScore(strRankKey, nMin, nMax, memberScoreVec);
}

int NFCRankRedisModule::GetRankMemberCount(const NFMsg::ERankType eRankType)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCard(strRankKey, nCount);

	return nCount;
}

int NFCRankRedisModule::GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCount(strRankKey, nMin, nMax, nCount);

	return nCount;
}

std::string NFCRankRedisModule::MakeRedisKey(const NFMsg::ERankType eRankType)
{
	std::string szKey = "Rank_" + ERankType_Name(eRankType);
	
	return szKey;
}
