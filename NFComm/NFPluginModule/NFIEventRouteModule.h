// -------------------------------------------------------------------------
//    @FileName      :    NFIEventRouteModule.h.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIEventRouteModule.h
//    @Desc			:	事件路由，主要用来代替分布式中有些对象不在本服务器然后需要转发的事件（邮件）
// -------------------------------------------------------------------------

#ifndef _NFI_EVENTROUTE_MODULE_H_
#define _NFI_EVENTROUTE_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIEventRouteModule
    : public NFILogicModule
{

public:
	//邮件，加好友申请，约占，等都可以最终路由成事件，再到目标服务器还原成逻辑申请
	//此时，由服务器逻辑决定这些对象跨不跨服务器聚集在一块
	virtual bool StartRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList) = 0;

	virtual bool ReceiveRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList) = 0;
};

#endif