// -------------------------------------------------------------------------
//    @FileName         £º    NFIProxyServerNet_ServerModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIProxyServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_SERVERMODULE_H
#define NFI_PROXYNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIProxyServerNet_ServerModule
    :  public NFILogicModule
{

public:
    virtual int Transpond(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID) = 0;
};

#endif