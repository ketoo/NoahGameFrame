// -------------------------------------------------------------------------
//    @FileName      :    NFIPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIPVPModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PVP_MODULE_H_
#define _NFI_PVP_MODULE_H_

#include <iostream>
#include "NFIScriptModule.h"

class NFIPVPModule
    : public NFILogicModule
{

public:
	virtual bool MatchPVPObject(const NFIDENTID& self) = 0;

	virtual bool StartPVPWar(const NFIDENTID& self) = 0;
	
	virtual bool ExitPVPWar(const NFIDENTID& self) = 0;

};

#endif