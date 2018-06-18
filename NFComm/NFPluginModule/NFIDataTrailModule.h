// -------------------------------------------------------------------------
//    @FileName			:    NFIDataTrailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIDataTrailModule
//
// -------------------------------------------------------------------------

#ifndef NFI_PROPERTY_TRAIL_MODULE_H
#define NFI_PROPERTY_TRAIL_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIDataTrailModule
    : public NFIModule
{
public:

	virtual void LogObjectData(const NFGUID& self) = 0;

    virtual void StartTrail(const NFGUID self) = 0;

};


#endif
