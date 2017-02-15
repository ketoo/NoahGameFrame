// -------------------------------------------------------------------------
//    @FileName			:    NFIShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFIShopModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_SHOP_MODULE_H
#define NFI_SHOP_MODULE_H

#include "NFIModule.h"

class NFIShopModule : public NFIModule
{
public:
	virtual bool ReqBuyItem(const NFGUID& self, const std::string& strID) = 0;
	virtual bool ReqBuyItem(const NFGUID& self, const std::string& strID, const NFVector3& v) = 0;
};

#endif