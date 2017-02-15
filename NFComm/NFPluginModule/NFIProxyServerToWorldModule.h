// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_CLIENTMODULE_H
#define NFI_PROXYNET_CLIENTMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.h"

class NFIProxyServerToWorldModule
    : public  NFIModule
{
public:
    virtual bool VerifyConnectData(const std::string& strAccount, const std::string& strKey) = 0;
    virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif