// -------------------------------------------------------------------------
//    @FileName      :   NFIItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#ifndef _NFI_ITEMCONSUME_PROCESS_MODULE_H_
#define _NFI_ITEMCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/Define/NFItem_def.h"

class NFIItemConsumeProcessModule
    : public NFILogicModule
{
public:

    //物品使用是否合法
    virtual int ConsumeLegal(const NFIDENTID& self, int nItemRowID,  const NFIValueList& other) = 0;

    //使用物品的消耗
    virtual int ConsumeSelf(const NFIDENTID& self, int nItemRowID) = 0;

    //合法,消耗,那么处理过程[消耗后,nItemRowID已经找不到了，因为可能被清空了]
    virtual int ConsumeProcess(const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other) = 0;

};

#endif