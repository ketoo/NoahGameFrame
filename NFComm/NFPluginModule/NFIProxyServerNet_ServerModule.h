// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_SERVERMODULE_H
#define NFI_PROXYNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFIProxyServerNet_ServerModule
    :  public NFIModule
{

public:
    virtual int Transpond(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID) = 0;
};

#endif