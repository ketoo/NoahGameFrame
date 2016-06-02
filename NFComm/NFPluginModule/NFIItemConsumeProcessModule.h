// -------------------------------------------------------------------------
//    @FileName      :   NFIItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#ifndef NFI_ITEMCONSUME_PROCESS_MODULE_H
#define NFI_ITEMCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemConsumeProcessModule
    : public NFILogicModule
{
public:

    //物品使用是否合法
    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID) = 0;

    //合法,消耗,那么处理过程[消耗后,nItemRowID已经找不到了，因为可能被清空了]
    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID) = 0;

};

#endif
