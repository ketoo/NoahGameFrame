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

struct SceneInfo
{
    NFList<int> mListNextScene;
    NFList<std::string> mListPassAward;
};


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
    virtual bool AddNewEctype(const NFIDENTID self) = 0;

    virtual bool AddEctypeActiveState(const NFIDENTID self, const int nContainerID) = 0;

    virtual bool CompleteEctypeMode(const NFIDENTID self, const int nContainerID, const int nStar) = 0;

    //能否进副本
    virtual bool CanEntryCloneScene(const NFIDENTID self, const int nContainerID) = 0;

    virtual int AfterOnBuyEliteTime(const NFIDENTID& self, const std::string& strConfig) = 0;

    virtual int OnEctypeSettleEvent(const NFIDENTID& self, int nResult, int nLevel) = 0; 

    virtual int AfterOnWipeScene(const NFIDENTID& self, int nWipeCount, int nItemCount,const std::string& strSceneId) = 0;

    virtual int OnWipeScene(const NFIDENTID& self, int nWipeCount, int nScene) = 0; 

    //购买精英副本次数
    virtual int OnBuyEliteTime(const NFIDENTID& self, int nScene ) = 0;

    virtual int ClearSceneTime(const NFIDENTID& self) = 0;
};

#endif