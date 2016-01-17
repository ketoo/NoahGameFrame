// -------------------------------------------------------------------------
//    @FileName         ��    NFIWorldNet_ClientModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIWorldNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLDNET_CLIENTMODULE_H
#define _NFI_WORLDNET_CLIENTMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"
#include "NFIClusterClientModule.hpp"

class NFIWorldToMasterModule
    : public NFIClusterClientModule
{

public:
	virtual int OnSelectServerResultsEvent(const int nWorldID, const NFGUID xSenderID, const int nLoginID, const std::string& strAccount, const std::string& strWorldIP, const int nWorldPort, const std::string& strWorldKey) = 0;

};

#endif