// -------------------------------------------------------------------------
//    @FileName			:    NFIHomeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-01-21
//    @Module           :    NFIHomeModule
//
// -------------------------------------------------------------------------

#ifndef NFI_HOME_MODULE_H
#define NFI_HOME_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIHomeModule
    : public NFIModule
{
public:
	virtual void Relive(const NFGUID& self) = 0;
};

#endif