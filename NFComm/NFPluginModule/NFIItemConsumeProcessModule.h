// -------------------------------------------------------------------------
//    @FileName      	:   NFIItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-02-16
//    @Module           :   NFIItemConsumeProcessModule

// -------------------------------------------------------------------------

#ifndef NFI_ITEM_CONSUME_PROCESS_MODULE_H
#define NFI_ITEM_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemConsumeProcessModule
    : public NFIModule
{
public:


    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID) = 0;


    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID) = 0;

};

#endif
