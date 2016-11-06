// -------------------------------------------------------------------------
//    @FileName         :    NFIRankRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIRankRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_RANK_REDIS_MODULE_H
#define NFI_RANK_REDIS_MODULE_H

#include "NFIModule.h"

class NFIRankRedisModule
    : public NFIModule
{

public:
	virtual bool SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value) = 0;
	virtual bool IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value) = 0;

	virtual bool RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID& self) = 0;
	virtual bool RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop) = 0;
	virtual bool RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax) = 0;

	virtual bool GetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, double& dwValue) = 0;
	virtual bool GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;
	virtual bool GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;

	virtual int GetRankMemberCount(const NFMsg::ERankType eRankType) = 0;
	virtual int GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax) = 0;

};

#endif