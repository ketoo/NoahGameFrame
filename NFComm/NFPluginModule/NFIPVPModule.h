// -------------------------------------------------------------------------
//    @FileName         :    NFIPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIPVPModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PVP_MODULE_H
#define _NFI_PVP_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFIPVPModule
    : public NFILogicModule
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

        int mnCellStatus;               // ����״̬ 0�п�λ 1��Ա 2
        NFMap<NFGUID, int> mxList;   // ����6����, first = playerID second = is_owner
    };

public:
    virtual bool MatchPVPObject(const NFGUID& self) = 0;

    virtual bool StartPVPWar(const NFGUID& self) = 0;

    virtual bool ExitPVPWar(const NFGUID& self) = 0;

};

#endif