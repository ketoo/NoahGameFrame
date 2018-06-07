// -------------------------------------------------------------------------
//    @FileName			:    NFCRankRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCRankRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_RANK_REDIS_MODULE_H
#define NFC_RANK_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIRankRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFCRankRedisModule : public NFIRankRedisModule
{
public:
	NFCRankRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	virtual bool SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value);
	virtual bool IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value);

	virtual bool RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID& self);
	virtual bool RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop);
	virtual bool RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax);

	virtual bool GetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, double& dwValue);
	virtual bool GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec);
	virtual bool GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec);

	virtual int GetRankMemberCount(const NFMsg::ERankType eRankType);
	virtual int GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax);

protected:
	std::string MakeRedisKey(const NFMsg::ERankType eRankType);

private:
	NFINoSqlModule* m_pNoSqlModule;

};


#endif
