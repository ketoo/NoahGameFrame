// -------------------------------------------------------------------------
//    @FileName      :    NFITotalLineTimeModule.h
//    @Author           :    LvSheng.Huang

//    @Module           :    NFITotalLineTimeModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TOTALTIME_MODULE_H
#define NFI_TOTALTIME_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFITotalLineTimeModule
    : public NFIModule
{
public:

    virtual int64_t GetTodayTotalOnlineTime(const NFGUID& self) = 0;
    virtual int64_t GetTodayTotalInCloneSceneTime(const NFGUID& self) = 0;
    virtual int64_t GetTodayMainLineCloneSceneTime(const NFGUID& self) = 0;
    virtual bool ClearCountTime(const NFGUID& self) = 0;

};


#endif
