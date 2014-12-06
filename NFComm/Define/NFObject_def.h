#pragma once

// -------------------------------------------------------------------------
//    文件名      :    NFSkill_def.h
//    创建者      :    LvSheng.Huang
//    创建时间    :    2010-10-5 12:05
//    功能描述    :    对象的结构体
//
// -------------------------------------------------------------------------

#include "NFItem_def.h"
//#include <vector>

#pragma pack(push,1)

////////////////////////////////////////////////////////////////////////////////

//性别
enum NFSexType
{
    NST_MAN,
    NST_FEMAN,
    NST_MAX,
};
//职业1:勇士2枪客3: 医4:弓手5刺客
enum NFJobType
{
    NJT_1,
    NJT_2,
    NJT_3,
    NJT_4,
    NJT_MAX,
};
//种族
enum NFRaceType
{
    NRT_1,
    NRT_2,
    NRT_3,
    NRT_4,
    NRT_MAX,
};
//阵营
enum NFCampType
{
    NCT_1,
    NCT_2,
    NCT_3,
    NCT_MAX,
};

//////////////////////////////////////////////////////////////////////////
//0呆着1跑2走3闪烁4技能飞移5死6平常技能中
enum NFObjectStateType
{
    NOST_IDLE,
    NOST_RUN,
    NOST_WALK,
    NOST_WILK,
    NOST_SKILLFLY,
    NOST_DEAD,
    NOST_SKILLUSE,
};

enum NFGMComandID
{
    NGMC_ID_ADDEXP,
    NGMC_ID_ADDITEM,
};






















#pragma pack(pop)