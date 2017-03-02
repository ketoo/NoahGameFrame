// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_LOGINNET_SERVERMODULE_H
#define NFI_LOGINNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFILoginNet_ServerModule
    : public NFIModule
{
public:

    virtual int OnSelectWorldResultsProcess(const int nWorldID, const NFGUID xSenderID, const int nLoginID, const std::string& strAccount, const std::string& strWorldIP, const int nWorldPort, const std::string& strKey) = 0;
    //virtual NFINetModule* GetNetModule() = 0;
};

#endif