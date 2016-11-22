// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_GAMESERVER_CONFIG_MODULE_H
#define NFI_GAMESERVER_CONFIG_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIGameServerConfigModule
    : public NFIModule
{
public:
    virtual bool AddSceneToActor(const int nSceneID, const int nGameServer) = 0;
    virtual int GetActorID(const int nSceneID) = 0;
};

#endif