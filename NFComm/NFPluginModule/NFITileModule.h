// -------------------------------------------------------------------------
//    @FileName			:    NFITileModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFITileModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TILE_MODULE_H
#define NFI_TILE_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFITileModule
    : public NFIModule
{
public:
	 virtual bool GetOnlinePlayerTileData(const NFGUID& self, std::string& strData) = 0;
};

#endif