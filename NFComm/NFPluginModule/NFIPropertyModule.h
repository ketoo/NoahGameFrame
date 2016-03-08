// -------------------------------------------------------------------------
//    @FileName         ：    NFIPropertyModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_MODULE_H
#define _NFI_PROPERTY_MODULE_H

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

        NPG_RUNTIME_BUFF    ,//动态BUFF

        NPG_ALL ,
    };


    virtual int RefreshBaseProperty(const NFGUID& self) = 0;
    virtual int GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int AddPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int SubPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;

    virtual bool FullHPMP(const NFGUID& self) = 0;
    virtual bool AddHP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeHP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughHP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddMP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeMP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughMP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool FullSP(const NFGUID& self) = 0;
    virtual bool AddSP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeSP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughSP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddMoney(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeMoney(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughMoney(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
};

#endif