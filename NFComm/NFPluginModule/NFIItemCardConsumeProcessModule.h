// -------------------------------------------------------------------------
//    @FileName      	:   NFIItemCardConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemCardConsumeProcessModule

// -------------------------------------------------------------------------

#ifndef NFI_ITEM_CARD_CONSUME_PROCESS_MODULE_H
#define NFI_ITEM_CARD_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemCardConsumeProcessModule
    : public NFIModule
{
public:


    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;


    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;

};

#endif
