// -------------------------------------------------------------------------
//    文件名      :    NFSkill_def.h
//    创建者      :    LvSheng.Huang
//    创建时间    :    2010-10-5 12:05
//    功能描述    :    技能的结构体
//
// -------------------------------------------------------------------------

#ifndef _NFC_SKILL_DEF_H_
#define _NFC_SKILL_DEF_H_

#pragma pack(push,1)


enum EGameSkillType
{
    EGST_JOBSKILL_BRIEF = 0,//普通短刃(可携带BUFF)
    EGST_JOBSKILL_BULLET = 1,//普通子弹(可携带BUFF)
    EGST_JOBSKILL_FLASH = 2,//普通闪烁(可携带BUFF)
    EGST_JOBSKILL_NEWOBJECT = 3,//普通召唤(坐标)(可携带BUFF)
};

enum EGameSkillConsumeType
{
    ConsumeType_Value           = 0, // 绝对值
    ConsumeType_CurrentPercent  = 1, // 当前百分比
    ConsumeType_MaxPercent      = 2, // 最大百分比
};

//攻击结果类型
enum EGameSkillResultsType
{
    EART_MISS = 0,        //MISS
    EART_NORMAL = 1,        //普通攻击正常
    EART_THUMP_BACK = 2,     //重击(击退)
    EART_THUMP_DOWN = 3,      //重击(击倒)
    EART_RESIST = 4,        //格挡
    EART_SUBSECTION2 = 5,   //分成2次连击
    EART_KILLEDMISS = 6,        //怪物被打没了。。。只能在怪物打了这下就死的情况下才用,客户端先隐藏
    EART_KILLEDNORMAL = 7        //怪物被打死
};

//尝试使用技能反馈(决定后面是追击还是释放-AI)
enum NFSkillTestSkillResult
{
    //返回0:可以 1:距离不够 2:CD不到
    NFSTSR_OK,
    NFSTSR_DISTANCE,
    NFSTSR_CD,
};

//技能表col值
enum EGameSkillStoreType
{
    EGSST_TYPE_SKILL_CONFIGID,//技能CONFIGID
    EGSST_TYPE_SKILL_LEVEL,//技能等级
    EGSST_TYPE_SKILL_GEM_CONFIGID,//附魂宝石
    EGSST_TYPE_SKILL_GEM_LEVEL,//附魂等级
};

#pragma pack(pop)
#endif