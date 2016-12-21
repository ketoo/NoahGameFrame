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
    struct PVPCell
    {
        enum PVPCellStatus
        {
            Status_HavePlace    = 0,
            Status_Full         = 1,
            Status_Start        = 2,
        };

        enum PVPCellEnum
        {
            PVPCell_Player_Count = 6,
        };



    };

public:
    virtual bool MatchPVPObject(const NFGUID& self) = 0;

    virtual bool StartPVPWar(const NFGUID& self) = 0;

    virtual bool ExitPVPWar(const NFGUID& self) = 0;

};

#endif