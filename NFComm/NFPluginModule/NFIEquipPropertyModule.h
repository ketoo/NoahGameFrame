// -------------------------------------------------------------------------
//    @FileName			:    NFIEquipPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEquipPropertyModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EQUIP_PROPERTY_MODULE_H
#define NFI_EQUIP_PROPERTY_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIEquipPropertyModule : public NFIModule
{
public:
	virtual bool CalEquipProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList) = 0;

};

#endif