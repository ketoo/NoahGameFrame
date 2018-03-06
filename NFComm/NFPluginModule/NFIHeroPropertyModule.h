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
	virtual bool CalFightintHeroProperty(const NFGUID& self) = 0;

};

#endif