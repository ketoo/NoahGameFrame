// -------------------------------------------------------------------------
//    @FileName         ��    NFINotifyModule.h
//    @Author           ��    qiaojun.ye
//    @Date             ��    2014-03-25
//    @Module           ��    NFNotifyModule
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