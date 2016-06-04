// -------------------------------------------------------------------------
//    @FileName         ：    NFIPackModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-10
//    @Module           ：    NFIPackModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PACK_MODULE_H
#define NFI_PACK_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

enum PackTableType
{
    NONE_PACK  = -1,
    BagItemPack  = 0, // 普通背包
    BagEquipPack  = 1, // 装备背包

    MaxPack,
};

class NFIPackModule : public NFILogicModule
{
public:
    virtual const std::string& GetPackName(const PackTableType name) = 0;
    /////////////基础/////////////////////////////////////////////////////////////

    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName) = 0;
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount) = 0;

    virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id) = 0;
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;

};

#endif