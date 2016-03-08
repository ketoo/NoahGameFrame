// -------------------------------------------------------------------------
//    @FileName         ��    NFIProxyServerNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIProxyServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_SERVERMODULE_H
#define _NFI_PROXYNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIProxyServerNet_ServerModule
    :  public NFINetModule
{

public:
    virtual int Transpond(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID) = 0;
};

#endif