// -------------------------------------------------------------------------
//    @FileName         £º    NFIActivityModule.h
//    @Author           £º    qiaojun.ye
//    @Date             £º    2014-04-01
//    @Module           £º    NFIActivityModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTIVITY_MODULE_H_
#define _NFI_ACTIVITY_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

enum
{
	AC_SIN_UP = 0,
};

class NFIActivityModule
	: public NFILogicModule
{
public:
	virtual bool ClearAcitivity(const NFIDENTID& self) = 0;


};

#endif