// -------------------------------------------------------------------------
//    @FileName			:    NFIDataTailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIDataTailModule
//
// -------------------------------------------------------------------------

#ifndef NFI_DATA_TAIL_MODULE_H
#define NFI_DATA_TAIL_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIDataTailModule
    : public NFIModule
{
public:

	virtual void LogObjectData(const NFGUID& self) = 0;

    virtual void StartTrail(const NFGUID& self) = 0;

};


#endif
