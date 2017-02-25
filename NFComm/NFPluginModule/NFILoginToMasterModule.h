// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_LOGINNET_CLIENTMODULE_H
#define NFI_LOGINNET_CLIENTMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.h"
#include "NFComm/NFMessageDefine/NFMsgPreGame.pb.h"

class NFILoginToMasterModule
    : public NFIModule
{
public:
	virtual NFINetClientModule* GetClusterModule() = 0;
    virtual NFMapEx<int, NFMsg::ServerInfoReport>& GetWorldMap() = 0;
    virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif
