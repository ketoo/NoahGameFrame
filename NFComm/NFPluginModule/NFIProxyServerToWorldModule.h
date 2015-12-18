// -------------------------------------------------------------------------
//    @FileName         £º    NFIProxyServerNet_ClientModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_CLIENTMODULE_H
#define _NFI_PROXYNET_CLIENTMODULE_H

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