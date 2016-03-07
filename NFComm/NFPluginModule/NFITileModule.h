// -------------------------------------------------------------------------
//    @FileName         :	NFITileModule.h
//    @Author           :	Nick Yang
//    @Date             :	2016/03/06
//    @Module           :	NFITileModule
// -------------------------------------------------------------------------
#ifndef NFI_TILE_MODULE_H
#define NFI_TILE_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFITileModule
    : public NFILogicModule
{
public:
    virtual void SetTileData(const NFGUID& self, const int x, const int y, const int type) = 0;

};

#endif // !NFI_TILE_MODULE_H