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
	virtual int OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroID) = 0;


};

#endif