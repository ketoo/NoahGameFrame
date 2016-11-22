// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PACK_MODULE_H
#define NFI_PACK_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIPackModule : public NFIModule
{
public:
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName) = 0;
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount) = 0;

    virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id) = 0;
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;

};

#endif