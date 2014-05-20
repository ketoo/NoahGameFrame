// -------------------------------------------------------------------------
//    @FileName         ：    NFIRelationConfigModule.h
//    @Author           ：    eliteYang
//    @Date             ：    2014-4-8
//    @Module           ：    NFIRelationConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_RELATION_CONFIG_MODULE_H_
#define _NFI_RELATION_CONFIG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIRelationConfigModule
    : public NFILogicModule
{
public:
    struct FriendlyLevelInfo
    {
    public:
        FriendlyLevelInfo() : nFriendly(0), nStringID(0)
        {
        }

        uint32_t nFriendly;
        uint32_t nStringID;
    };

    typedef std::vector<FriendlyLevelInfo> FriendlyLevelInfoVector;
    typedef FriendlyLevelInfoVector::iterator FriendlyLevelInfoVectorIter;
    typedef FriendlyLevelInfoVector::const_iterator FriendlyLevelInfoVectorCIter;
    typedef FriendlyLevelInfoVector::const_reverse_iterator FriendlyLevelInfoVectorCRIter;

    struct FriendlySetting
    {
        FriendlySetting() : nMaxFriendly(0), nMaxFriendlylevel(0)
        {
            vecFriendlyLevelInfo.clear();
        }

        uint32_t nMaxFriendly;
        uint16_t  nMaxFriendlylevel;
        FriendlyLevelInfoVector vecFriendlyLevelInfo;
    };

    // 一些限制
    struct RelationLimitSetting
    {
        RelationLimitSetting() : nMaxTempFriendCount(100), nMaxFriendCount(20), nMaxBlackListCount(50), nMaxEnemyCount(50)
        {

        }

        uint16_t nMaxTempFriendCount;
        uint16_t nMaxFriendCount;
        uint16_t nMaxBlackListCount;
        uint16_t nMaxEnemyCount;
    };

    // 好友召唤限定
    struct SummonData 
    {
        SummonData() : nCount(0), nCostMoney(0), nCostYuanBao(0)
        {
        }

        uint16_t nCount;
        uint16_t nCostMoney;
        uint16_t nCostYuanBao;
    };

    typedef std::map<uint16_t, SummonData> SummonDataMap;
    typedef SummonDataMap::iterator SummonDataMapIter;

    struct SummonSetting 
    {
        SummonSetting() : nSummonCount(0), nSummonTime(0)
        {
            summonDataMap.clear();
        }

        uint16_t nSummonCount;
        uint16_t nSummonTime;
        SummonDataMap summonDataMap;
    };

public:
    virtual uint16_t GetSummonCount() const = 0;
    virtual NFIRelationConfigModule::SummonData* GetSummonData(uint16_t nCount) = 0;
    virtual uint16_t GetFriendMaxCount() const = 0;

private:
};

#endif // !_NFI_RELATION_CONFIG_MODULE_H_