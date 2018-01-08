// -------------------------------------------------------------------------
//    @FileName         :    NFIPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIPVPModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PVP_MODULE_H
#define NFI_PVP_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIPVPModule
    : public NFIModule
{
public:
    enum PVP_TYPE
    {
        PVP_HOME,
        PVP_INDIVIDUAL,
        PVP_DOTA,
        PVP_SURVIVAL,
        PVP_GUILD_WAR,
        PVP_GUILD_DUNGEON,
    };

	virtual int RandomTileScene() = 0;

};

#endif