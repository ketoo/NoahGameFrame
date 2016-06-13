// -------------------------------------------------------------------------
//    @FileName         ：    NFIBuffModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIBuffModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LEVEL_MODULE_H
#define NFI_LEVEL_MODULE_H

#include <iostream>
#include "NFILogicModule.h"


class NFILevelModule
    : public NFILogicModule
{
public:
    //nType:增加原因
    virtual int AddExp(const NFGUID& self, const int nExp) = 0;

};

#endif