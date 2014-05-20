// -------------------------------------------------------------------------
//    @FileName         ：    NFIPropertyModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_MODULE_H_
#define _NFI_PROPERTY_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIPropertyModule
    : public NFILogicModule
{
public:
    enum NFPropertyGroup
    {
        NPG_JOBLEVEL        ,//职业等级基础
        NPG_EFFECTVALUE     ,//一级属性影响二级属性的部分(直接用公式计算，其他部分不用动)
        NPG_REBIRTH_ADD     ,//重生奖励
        NPG_EQUIP           ,//装备不会影响体质之类的导致2次计算(直接不要力量体质什么的)
        NPG_EQUIP_AWARD     ,//套装奖励
        NPG_STATIC_BUFF     ,//永久类型的BUFF，天赋之类的，或者吃的永久药水

        NPG_PET             ,//当前出战的副将基础属性--出战后貌似会替代主角基础属性
        NPG_ONAGER          ,//怒炮影响的属性

        NPG_MOUNT           ,// 坐骑

        NPG_BIRD_BUFF       ,//天鹰影响BUFF

        NPG_RUNTIME_BUFF    ,//动态BUFF

        NPG_ALL = 100       ,
    };


    virtual int RefreshBaseProperty(const NFIDENTID& self) = 0;
    virtual int GetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int AddPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int SubPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;

    virtual int FullHPMP(const NFIDENTID& self) = 0;

    virtual int AddHP(const NFIDENTID& self, int nValue)  = 0;
    virtual int ConsumeHP(const NFIDENTID& self, int nValue) = 0;

    virtual int AddMP(const NFIDENTID& self, int nValue) = 0;
    virtual int ConsumeMP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeSP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullSP(const NFIDENTID& self) = 0;
    virtual int AddSP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeVP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullVP(const NFIDENTID& self) = 0;
    virtual int RestoreVP( const NFIDENTID& self ) = 0;
    virtual int AddVP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeActivityVP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullActivityVP(const NFIDENTID& self) = 0;
    virtual int AddActivityVP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeMoney(const NFIDENTID& self, int nValue) = 0;
    virtual int AddMoney(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeYBP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeSoulStone(const NFIDENTID& self, int nValue) = 0;
    virtual int AddSoulStone(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeEssence(const NFIDENTID& self, int nValue) = 0;
    virtual int AddEssence(const NFIDENTID& self, int nValue) = 0;

	virtual int AddHonour(const NFIDENTID& self, int nValue) = 0;
	virtual int ConsumeHonour(const NFIDENTID& self, int nValue) = 0;
};

#endif