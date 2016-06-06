// -------------------------------------------------------------------------
//    @FileName         £º    NFILoginNet_ClientModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFILoginNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGINNET_CLIENTMODULE_H
#define NFI_LOGINNET_CLIENTMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFIClusterClientModule.hpp"
#include "NFComm/NFMessageDefine/NFMsgPreGame.pb.h"

class NFILoginToMasterModule
    : public NFILogicModule
{
public:
	virtual NFIClusterClientModule* GetClusterModule() = 0;
    virtual NFMapEx<int, NFMsg::ServerInfoReport>& GetWorldMap() = 0;
};

#endif
