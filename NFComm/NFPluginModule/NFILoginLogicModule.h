// -------------------------------------------------------------------------




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
    virtual void OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;

};

#endif