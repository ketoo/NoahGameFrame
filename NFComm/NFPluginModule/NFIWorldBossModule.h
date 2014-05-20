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
        int nRate;                      // ��������
        int nMinLevel;                  // ��������С��ɫ�ȼ�
        int nMaxLevel;                  // ����������ɫ�ȼ�
        int nLifeTime;                  // ����ʱ��
        int nMaxPushTimes;              // ������ʹ���
        int nPushCount;                 // ÿ����������
        int nJoinCount;                 // �μӵ�����
        int nTriggerScore;              // ������������
        int nJoinScore;                 // �μӽ�������
        int nKillScore;                 // ��ɱ��������
        int nMVPScore;                  // MVP��������
    };
};

#endif // !_NFI_WORLD_BOSS_MODULE_H_