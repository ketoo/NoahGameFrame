// -------------------------------------------------------------------------
//    @FileName         ��    NFIBuffModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIBuffModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LEVELCONFIG_MODULE_H_
#define _NFI_LEVELCONFIG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"


class NFILevelConfigModule
    : public NFILogicModule
{
public:

    virtual int GetNeedExp(int nLevel) = 0;
};

#endif