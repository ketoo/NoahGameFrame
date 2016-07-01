// -------------------------------------------------------------------------
//    @FileName         :    NFIPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIPVPModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PVP_MATCH_MODULE_H
#define NFI_PVP_MATCH_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFIPVPMatchModule
    : public NFILogicModule
{
public:
    enum  EPVPMODE
    {
        EPVPMODE_ONE, //1v1
        EPVPMODE_TWO, //2v2
        EPVPMODE_THREE, //3v3
    };

    enum  EPVPROOMSTATUS
    {
        EPVPROOMSTATUS_WAIT             = 0, //
        EPVPROOMSTATUS_WAITFIGHT        = 1, //
        EPVPROOMSTATUS_WAITCREATEECTYPE = 2, //
        EPVPROOMSTATUS_FIGHT            = 3, //
        EPVPROOMSTATUS_FINISH           = 4, //
    };

    enum  EPVPDefine
    {
        EPVPDEFINE_MAXGRADE = 10, //
    };

    enum  EPVPREDORBLUE
    {
        EPVPREDORBLUE_RED = 0, //
        EPVPREDORBLUE_BULE = 1, //
    };

public:
    struct PVPRoom
    {
        PVPRoom()
        {
            mnCellStatus = 0;
            mnPVPMode = EPVPMODE_ONE;
            nMaxPalyer = 1;
        }

        int mnCellStatus;             // 状态
        NFGUID xCellID;               // 房间ID
        int mnPVPMode;                // 匹配模式
        int mnPVPGrade;               // 房间所在的阶段
        int nMaxPalyer;
        NFMapEx<NFGUID, int> mxRedPlayer;
        NFMapEx<NFGUID, int> mxBluePlayer;
    };

public:
};

#endif