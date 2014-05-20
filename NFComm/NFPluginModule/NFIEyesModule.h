// -------------------------------------------------------------------------
//    @FileName         £º    NFIEyesModule.h
//    @Author           £º    qiaojun.ye
//    @Date             £º    2014-03-10
//    @Module           £º    NFIEyesModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_EYES_MODULE_H_
#define _NFI_EYES_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

enum EyesList
{
	EYES_CONFIG,
	EYES_WISH,
	EYES_TIMES,
};

const std::string strEyesList =  "EyesList";
class NFIEyesModule
	: public NFILogicModule
{

public:
	virtual bool ClearEyesWish(const NFIDENTID& self) = 0;
    virtual bool AddNewerEyes(const NFIDENTID& self, const int nLevel) = 0;
	virtual bool AddEyes(const NFIDENTID& self, const std::string& strEyesConfig) = 0;
};

#endif