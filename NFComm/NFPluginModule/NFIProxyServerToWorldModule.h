// -------------------------------------------------------------------------
//    @FileName         £º    NFIProxyServerNet_ClientModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_CLIENTMODULE_H
#define NFI_PROXYNET_CLIENTMODULE_H

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