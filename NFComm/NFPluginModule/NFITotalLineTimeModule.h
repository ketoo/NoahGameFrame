// -------------------------------------------------------------------------
//    @FileName      :    NFITotalLineTimeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             £º    2013-12-03
//    @Module           :    NFITotalLineTimeModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_TOTALTIME_MODULE_H_
#define _NFI_TOTALTIME_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFITotalLineTimeModule
    : public NFILogicModule
{
public:

    virtual int64_t GetTodayTotalOnlineTime(const NFIDENTID& self) = 0;
    virtual int64_t GetTodayTotalInCloneSceneTime(const NFIDENTID& self) = 0;
	virtual int64_t GetTodayMainLineCloneSceneTime(const NFIDENTID& self) = 0;
	virtual bool ClearCountTime(const NFIDENTID& self) = 0;

};


#endif
