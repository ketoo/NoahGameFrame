// -------------------------------------------------------------------------
//    @FileName         £º    NFINotifyModule.h
//    @Author           £º    qiaojun.ye
//    @Date             £º    2014-03-25
//    @Module           £º    NFNotifyModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_NOTIFY_MODULE_H_
#define _NFI_NOTIFY_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

enum NOTIFY_TYPE
{


};




class NFINotifyModule
	: public NFILogicModule
{

public:

	virtual bool BroadCastNotify(const NFIDENTID& self, const NFCValueList& var) = 0;

};

#endif