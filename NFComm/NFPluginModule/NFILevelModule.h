// -------------------------------------------------------------------------
//    @FileName         ��    NFIBuffModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIBuffModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LEVEL_MODULE_H_
#define _NFI_LEVEL_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"


class NFILevelModule
    : public NFILogicModule
{

public:

    //nType:����ԭ��
    virtual int AddExp(const NFIDENTID& self, const int nExp) = 0;

    //����ʱ
    virtual int OnKillObject(const NFIDENTID& self, const NFIDENTID& other) = 0;

    //����ʱ
    virtual int OnDead(const NFIDENTID& self, const NFIDENTID& other) = 0;

};

#endif