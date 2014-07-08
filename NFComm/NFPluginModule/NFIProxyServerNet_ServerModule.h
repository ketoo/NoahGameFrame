// -------------------------------------------------------------------------
//    @FileName         ��    NFIProxyServerNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIProxyServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_SERVERMODULE_H_
#define _NFI_PROXYNET_SERVERMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIProxyServerNet_ServerModule
    :  public NFINetModule
{

public:

    virtual int Transpond(const NFIPacket& msg) = 0;

};

#endif