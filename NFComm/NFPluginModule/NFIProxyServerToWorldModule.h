// -------------------------------------------------------------------------
//    @FileName         ��    NFIProxyServerNet_ClientModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_CLIENTMODULE_H_
#define _NFI_PROXYNET_CLIENTMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFIClusterClientModule.hpp"

class NFIProxyServerToWorldModule
    : public  NFIClusterClientModule
{


public:
    virtual bool VerifyConnectData(const std::string& strAccount, const std::string& strKey) = 0;
};

#endif