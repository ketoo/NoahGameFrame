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
    //运行时能分层的属性--其实是为record的tag作用
//     enum NFRumTimeColIndex
//     {
//         MAXHP,
//         MAXMP,
//         MAXSP,
//         HPREGEN,
//         SPREGEN,
//         MPREGEN,
//         ATK_VALUE,
//         DEF_VALUE,
//         MOVE_SPEED,
//         ATK_SPEED,
//         ATK_FIRE,
//         ATK_LIGHT,
//         ATK_ICE,
//         ATK_POISON,
//         DEF_FIRE,
//         DEF_LIGHT,
//         DEF_ICE,
//         DEF_POISON,
//         DIZZY_GATE,
//         MOVE_GATE,
//         SKILL_GATE,
//         PHYSICAL_EGATE,
//         MAGIC_EGATE,
//         BUFF_EGATE,
// 
//         NFRTC_END,
//     };

    virtual int CalculateBaseValue(const NFJobType jobType,  const int nLevel, const std::string& strProperty) = 0;
    virtual bool NeedView(const std::string& strProperty) = 0;
};


#endif
