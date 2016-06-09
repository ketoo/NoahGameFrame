// -------------------------------------------------------------------------
//    @FileName         :	NFIHeroPropertyModule.h
//    @Author           :	Lvsheng.Huang
//    @Date             :	2012-12-15
//    @Module           :	NFIHeroPropertyModule
// -------------------------------------------------------------------------
#ifndef NFI_HERO_PROPERTY_MODULE_H
#define NFI_HERO_PROPERTY_MODULE_H


#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIHeroPropertyModule
	: public NFILogicModule
{
public:

	virtual bool OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroGUID) = 0;
	
	virtual bool CalHeroAllProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList) = 0;
	virtual bool CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList) = 0;
	virtual bool CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList) = 0;
	virtual bool CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList) = 0;


};

#endif