// -------------------------------------------------------------------------
//    @FileName         :   NFIWorldBossModule.h
//    @Author           :   eliteYang
//    @Date             :   2014/01/22
//    @Module           :   NFCWorldBossModule
//    @Mail             :   elite_yang@163.com
//
// -------------------------------------------------------------------------
#ifndef _NFI_WORLD_BOSS_MODULE_H_
#define _NFI_WORLD_BOSS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIWorldBossModule
    : public NFILogicModule
{
public:
    struct CommonData 
    {
        CommonData() : nStartYear(0), nStartMonth(0), nStartDay(0), nStartHour(0), nStartMinute(0),
            nEndYear(0), nEndMonth(0), nEndDay(0), nEndHour(0), nEndMinute(0)
        {
        }

        int nStartYear;
        int nStartMonth;
        int nStartDay;
        int nStartHour;
        int nStartMinute;

        int nEndYear;
        int nEndMonth;
        int nEndDay;
        int nEndHour;
        int nEndMinute;
    };

    struct WorldBossData 
    {
        WorldBossData() : strWorldBossIndex(""), strMonsterID(""), nRate(0), nMinLevel(0), nMaxLevel(0),
            nLifeTime(0), nMaxPushTimes(0), nPushCount(0), nJoinCount(0), nTriggerScore(0),
            nJoinScore(0), nKillScore(0), nMVPScore(0)
        {

        }

        std::string strWorldBossIndex;  // boss index
        std::string strMonsterID;       // boss ID
        int nRate;                      // 触发概率
        int nMinLevel;                  // 触发的最小角色等级
        int nMaxLevel;                  // 触发的最大角色等级
        int nLifeTime;                  // 生存时间
        int nMaxPushTimes;              // 最大推送次数
        int nPushCount;                 // 每次推送人数
        int nJoinCount;                 // 参加的人数
        int nTriggerScore;              // 触发奖励积分
        int nJoinScore;                 // 参加奖励积分
        int nKillScore;                 // 击杀奖励积分
        int nMVPScore;                  // MVP奖励积分
    };
};

#endif // !_NFI_WORLD_BOSS_MODULE_H_