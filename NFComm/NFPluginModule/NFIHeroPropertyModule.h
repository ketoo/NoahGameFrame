// -------------------------------------------------------------------------
//    @FileName         :	NFIHeroPropertyModule.h
//    @Author           :	Lvsheng.Huang
//    @Date             :	2012-12-15
//    @Module           :	NFIHeroPropertyModule
// -------------------------------------------------------------------------
#ifndef NFI_HERO_PROPERTY_MODULE_H
#define NFI_HERO_PROPERTY_MODULE_H


#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFIHeroPropertyModule
	: public NFIModule
{
public:

	virtual bool OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroGUID) = 0;
	
	virtual bool CalHeroAllProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList) = 0;
	virtual bool CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList) = 0;
	virtual bool CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList) = 0;
	virtual bool CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList) = 0;

	virtual bool FullHPMP(const NFGUID& self, const NFGUID& xHeroGUID) = 0;
	virtual bool AddHP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;
	virtual bool ConsumeHP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;
	virtual bool EnoughHP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;

	virtual bool AddMP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;
	virtual bool ConsumeMP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;
	virtual bool EnoughMP(const NFGUID& self, const NFGUID& xHeroGUID, const NFINT64& nValue) = 0;

};

#endif