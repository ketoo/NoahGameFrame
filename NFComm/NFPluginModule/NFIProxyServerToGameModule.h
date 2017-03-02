// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_PROXYNET_TO_GAME_H
#define NFI_PROXYNET_TO_GAME_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.h"

class NFIProxyServerToGameModule
    : public  NFIModule
{
public:
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif