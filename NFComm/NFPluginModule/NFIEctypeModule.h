// -------------------------------------------------------------------------
//    @FileName         ：    NFIEctypeModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIEctypeModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_ECTYPE_MODULE_H_
#define _NFI_ECTYPE_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIEctypeModule
    : public NFILogicModule
{
public:

    enum EctypeColState
    {
        ECS_ON_OPEN,
        ECS_HAS_OPEN,
        ECS_HAS_PASS,
    };

    enum EctypeRecordCol
    {
        EXTYPE_RC_SCENEID,
        EXTYPE_RC_STATE,
        EXTYPE_RC_STAR,
    };

public:
    virtual bool CompleteEctypeMode(const NFIDENTID self, const int nContainerID, const int nStar) = 0;
    virtual int OnEctypeSettleEvent(const NFIDENTID& self, int nResult, int nLevel) = 0;
    //能否进副本
    virtual bool CanEntryCloneScene(const NFIDENTID self, const int nContainerID) = 0;
};

#endif