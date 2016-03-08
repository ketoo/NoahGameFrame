// -------------------------------------------------------------------------
//    @FileName      :    NFITotalLineTimeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             £º    2013-12-03
//    @Module           :    NFITotalLineTimeModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_TOTALTIME_MODULE_H
#define _NFI_TOTALTIME_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFITotalLineTimeModule
    : public NFILogicModule
{
public:

    virtual int64_t GetTodayTotalOnlineTime(const NFGUID& self) = 0;
    virtual int64_t GetTodayTotalInCloneSceneTime(const NFGUID& self) = 0;
    virtual int64_t GetTodayMainLineCloneSceneTime(const NFGUID& self) = 0;
    virtual bool ClearCountTime(const NFGUID& self) = 0;

};


#endif
