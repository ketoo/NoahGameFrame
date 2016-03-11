// -------------------------------------------------------------------------
//    @FileName         ��    NFILoginNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFILoginNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGINLOGIC_MODULE_H
#define _NFI_LOGINLOGIC_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFILoginLogicModule
    : public NFILogicModule
{
public:
    virtual int OnLoginProcess(const NFGUID& object, const std::string& strAccount, const std::string& strPwd) = 0;

};

#endif