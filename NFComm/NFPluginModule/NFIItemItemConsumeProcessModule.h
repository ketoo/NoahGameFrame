// -------------------------------------------------------------------------
//    @FileName      	:   NFIItemItemConsumeProcessModule.h
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFIItemItemConsumeProcessModule

// -------------------------------------------------------------------------

#ifndef NFI_ITEM_ITEM_CONSUME_PROCESS_MODULE_H
#define NFI_ITEM_ITEM_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemItemConsumeProcessModule
    : public NFIModule
{
public:


    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;


    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;

};

#endif
