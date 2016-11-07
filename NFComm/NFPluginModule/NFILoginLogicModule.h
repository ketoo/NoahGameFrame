// -------------------------------------------------------------------------
//    @FileName         £º    NFILoginNet_ServerModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFILoginNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGINLOGIC_MODULE_H
#define NFI_LOGINLOGIC_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFILoginLogicModule
    : public NFIModule
{
public:
    virtual void OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;

};

#endif