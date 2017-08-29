// -------------------------------------------------------------------------
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27
//    @Module           :    NFIRankModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_RANK_MODULE_H
#define NFI_RANK_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFIRankModule
    : public NFIModule
{

public:
    enum RANK_TYPE
    {
        RT_INDIVIDUAL_LEVEL,
        RT_INDIVIDUAL_MONEY,
        RT_GUILD_LEVEL,
        RT_GUILD_MONEY,
    };

    struct RankValue
    {
        int index;
        int score;
        NFGUID id;
    };

    virtual void AddValue(const NFGUID& self, const RANK_TYPE type, NFINT64 value) = 0;
    virtual void SetValue(const NFGUID& self, const RANK_TYPE type, NFINT64 value) = 0;
    virtual void SubValue(const NFGUID& self, const RANK_TYPE type, NFINT64 value) = 0;

    virtual NFIRankModule::RankValue GetIndex(const NFGUID& self, const NFIRankModule::RANK_TYPE type) = 0;

    virtual int RangeByIndex(const NFINT64 startIndex, const NFINT64 endIndex, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector) = 0;
    virtual int RangeByScore(const NFINT64 startScore, const NFINT64 endScore, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector) = 0;

};

#endif