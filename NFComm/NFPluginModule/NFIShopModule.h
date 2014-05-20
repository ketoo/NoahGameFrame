// -------------------------------------------------------------------------
//    @FileName         £º    NFIShopModule.h
//    @Author           £º    qiaojun.ye
//    @Date             £º    2014-03-03
//    @Module           £º    NFIShopModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SHOP_MODULE_H_
#define _NFI_SHOP_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIShopModule
	: public NFILogicModule
{

public:

	virtual bool ClearDayCount(const NFIDENTID& self) = 0;

};

#endif