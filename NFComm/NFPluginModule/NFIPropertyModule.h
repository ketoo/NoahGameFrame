// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PROPERTY_MODULE_H
#define NFI_PROPERTY_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIPropertyModule
    : public NFIModule
{
public:
    enum NFPropertyGroup
    {
		NPG_JOBLEVEL,
		NPG_EFFECTVALUE,
		NPG_REBIRTH_ADD,
		NPG_EQUIP,
		NPG_EQUIP_AWARD,
		NPG_STATIC_BUFF,
		NPG_RUNTIME_BUFF,
        NPG_ALL ,
    };


    virtual int RefreshBaseProperty(const NFGUID& self) = 0;
    virtual int64_t GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue) = 0;
    virtual bool CalculatePropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue, const bool bPositive = false) = 0;

    virtual bool FullHPMP(const NFGUID& self) = 0;
    virtual bool AddHP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeHP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughHP(const NFGUID& self, const int nValue) = 0;
	virtual bool DamageHP(const NFGUID& self, const int nValue) = 0;

    virtual bool AddMP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeMP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughMP(const NFGUID& self, const int nValue) = 0;
	virtual bool DamageMP(const NFGUID& self, const int nValue) = 0;

    virtual bool FullSP(const NFGUID& self) = 0;
    virtual bool AddSP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeSP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughSP(const NFGUID& self, const int nValue) = 0;

    virtual bool AddGold(const NFGUID& self, const int64_t nValue) = 0;
    virtual bool ConsumeGold(const NFGUID& self, const int64_t nValue) = 0;
    virtual bool EnoughGold(const NFGUID& self, const int64_t nValue) = 0;

    virtual bool AddDiamond(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeDiamond(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughDiamond(const NFGUID& self, const int nValue) = 0;
};

#endif