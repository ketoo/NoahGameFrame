// -------------------------------------------------------------------------
//    @FileName         :    NFIPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIPVPModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PVP_MODULE_H_
#define _NFI_PVP_MODULE_H_

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

        int mnCellStatus;               // 房间状态 0有空位 1满员 2
        NFMap<NFIDENTID, int> mxList;   // 仅限6个人, first = playerID second = is_owner
    };

public:
	virtual bool MatchPVPObject(const NFIDENTID& self) = 0;

	virtual bool StartPVPWar(const NFIDENTID& self) = 0;
	
	virtual bool ExitPVPWar(const NFIDENTID& self) = 0;

};

#endif