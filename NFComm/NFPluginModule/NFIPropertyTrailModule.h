// -------------------------------------------------------------------------
//    @FileName      :    NFIPropertyTrailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIPropertyTrailModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_TRAIL_MODULE_H_
#define _NFI_PROPERTY_TRAIL_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIPropertyTrailModule
    : public NFILogicModule
{
public:
	virtual void StartTrail(const NFIDENTID self) = 0;
	virtual void EndTrail(const NFIDENTID self) = 0;

};


#endif
