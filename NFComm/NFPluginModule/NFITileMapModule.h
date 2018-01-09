// -------------------------------------------------------------------------
//    @FileName			:    NFITileMapModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFITileMapModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TILE_MAP_MODULE_H
#define NFI_TILE_MAP_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFITileMapModule
    : public NFIModule
{
public:
	virtual bool GenerateTileMap(const NFGUID self, const NFGUID other, const int nSceneID) = 0;
	virtual bool ReleaseTileMap(const NFGUID self) = 0;


};

#endif