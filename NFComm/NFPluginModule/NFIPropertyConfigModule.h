// -------------------------------------------------------------------------
//    @FileName      :    NFIPropertyConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIPropertyConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_CONFIG_MODULE_H_
#define _NFI_PROPERTY_CONFIG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIPropertyConfigModule
    : public NFILogicModule
{
public:
    //运行时能分层的属性
    enum NFRumTimeColIndex
    {
        MAXHP,
        MAXMP,
        MAXSP,
        MAXVP,
        MAXActivityVP,
        HPREGEN,
        SPREGEN,
        MPREGEN,
        VPREGEN,
        ACTIVETYVPREGEN,
        EXP_SCALE,
        GOLD_SCALE,
        EQUIP_SCALE,
        HIT_VALUE,
        ESSENCE_SCALE,
        DODGE_VALUE,
        CRITICAL_VALUE,
        BROKEN_VALUE,
        DAMAGE_REFOUND,
        DAMAGE_GETHP,
        RESIST_VALUE,
        ATK_VALUE,
        DEF_VALUE,
        CRITICAL_RATIO_VALUE,
        DEF_CRITICAL_RATIO_VALUE,
        DEF_HALT_VALUE,
        DEF_SLOW_VALUE,
        DEF_SILENCE_VALUE,
        DEF_DIZZY_VALUE,
        DEF_FREZE_VALUE,
        MOVE_SPEED,
        ATK_SPEED,
        ATK_FIRE,
        ATK_LIGHT,
        ATK_ICE,
        ATK_POISON,
        DEF_FIRE,
        DEF_LIGHT,
        DEF_ICE,
        DEF_POISON,
        MAGIC_BREAK,
        PHYSICAL_BREAK,
        MAGIC_REFLECTION,
        PHYSICAL_REFLECTION,
        DIZZY_GATE,
        MOVE_GATE,
        SKILL_GATE,
        PHYSICAL_EGATE,
        MAGIC_EGATE,
        BUFF_EGATE,

        NFRTC_END,
    };

    virtual int CalculateBaseValue(const NFJobType jobType,  const int nLevel, const std::string& strProperty) = 0;
    virtual bool NeedView(const std::string& strProperty) = 0;
};


#endif
