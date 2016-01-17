// -------------------------------------------------------------------------
//    @FileName         £º    NFILoginNet_ClientModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFILoginNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGINNET_CLIENTMODULE_H
#define _NFI_LOGINNET_CLIENTMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFIClusterClientModule.hpp"
#include "NFComm/NFMessageDefine/NFMsgPreGame.pb.h"

class NFILoginToMasterModule
    : public NFIClusterClientModule
{

public:
    virtual NFMapEx<int, NFMsg::ServerInfoReport>& GetWorldMap() = 0;
};

#endif
