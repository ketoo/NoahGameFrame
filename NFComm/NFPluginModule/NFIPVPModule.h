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

	virtual int RandomTileScene() = 0;

};

#endif