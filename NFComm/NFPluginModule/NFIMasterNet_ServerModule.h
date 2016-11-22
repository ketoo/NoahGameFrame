// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_MASTERNET_SERVERMODULE_H
#define NFI_MASTERNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIMasterNet_ServerModule
    : public NFIModule
{

public:
	virtual std::string GetServersStatus() = 0;
};

#endif