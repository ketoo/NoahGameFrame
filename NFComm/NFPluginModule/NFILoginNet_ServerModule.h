// -------------------------------------------------------------------------
//    @FileName         ��    NFILoginNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFILoginNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGINNET_SERVERMODULE_H
#define NFI_LOGINNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFILoginNet_ServerModule
    : public NFINetModule
{
public:

    virtual int OnSelectWorldResultsProcess(const int nWorldID, const NFGUID xSenderID, const int nLoginID, const std::string& strAccount, const std::string& strWorldIP, const int nWorldPort, const std::string& strKey) = 0;

};

#endif