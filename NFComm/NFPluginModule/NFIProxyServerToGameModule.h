// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_TO_GAME_H
#define NFI_PROXYNET_TO_GAME_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.hpp"

class NFIProxyServerToGameModule
    : public  NFIModule
{
public:
	virtual NFINetClientModule* GetClusterModule() = 0;
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif