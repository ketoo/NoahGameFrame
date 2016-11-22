// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_BUFF_MODULE_H
#define NFI_BUFF_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFIBuffConfigModule.h"


class NFIBuffModule
    : public NFIModule
{

public:

    virtual bool AddBuff(const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent) = 0;


    virtual bool RemoveBuff(const NFGUID& self, const std::string& strConfig) = 0;


    virtual bool RemoveBuff(const NFGUID& self, NFIBuffConfigModule::BuffType eType) = 0;


    virtual bool SelfFree(const NFGUID& self) = 0;


    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty) = 0;

    virtual bool HasBuff(const NFGUID& self, const std::string& strConfig) = 0;


    virtual bool Execute(const NFGUID& self, float fPassTime) = 0;


    virtual int Attack(const NFGUID& self, const NFGUID& other) = 0;


    virtual int OnDead(const NFGUID& self) = 0;

};

#endif