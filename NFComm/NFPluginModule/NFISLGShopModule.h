// -------------------------------------------------------------------------
//    @FileName         :    NFISLGShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFISLGShopModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SLG_SHOP_MODULE_H_
#define _NFI_SLG_SHOP_MODULE_H_

#include "NFILogicModule.h"

class NFISLGShopModule
    : public NFILogicModule
{
public:
	virtual bool ReqBuyItem(const NFIDENTID& self, const std::string& strID, float fX, float fY, float fZ) = 0;
};

#endif