// -------------------------------------------------------------------------
//    @FileName         ：    NFIBuffModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIBuffModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LEVEL_MODULE_H
#define _NFI_LEVEL_MODULE_H

#include <iostream>
#include "NFILogicModule.h"


class NFILevelModule
    : public NFILogicModule
{

public:

    //nType:增加原因
    virtual int AddExp(const NFGUID& self, const int nExp) = 0;

    //死亡时
    virtual int OnKillObject(const NFGUID& self, const NFGUID& other) = 0;

    //死亡时
    virtual int OnDead(const NFGUID& self, const NFGUID& other) = 0;

};

#endif