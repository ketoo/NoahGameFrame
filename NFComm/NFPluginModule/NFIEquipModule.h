// -------------------------------------------------------------------------
//    @FileName         £º    NFIEquipModule.h
//    @Author           £º    chuanbo.guo
//    @Date             £º    2016-01-12
//    @Module           £º    NFIEquipModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EQUIP_MODULE_H
#define NFI_EQUIP_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIEquipModule : public NFILogicModule
{
public:
    virtual bool IntensifylevelToEquip(const NFGUID& self, const NFGUID& xEquipID) = 0;
    virtual bool HoleToEquip(const NFGUID& self, const NFGUID& xEquipID) = 0;
    virtual bool InlaystoneToEquip(const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex) = 0;
    virtual bool ElementlevelToEquip(const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType) = 0;
    virtual bool WearEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget) = 0;
    virtual bool TakeOffEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget) = 0;
};

#endif