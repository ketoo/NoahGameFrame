// -------------------------------------------------------------------------
//    @FileName      	:   NFIItemTokenConsumeProcessModule.h
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFIItemTokenConsumeProcessModule

// -------------------------------------------------------------------------

#ifndef NFI_ITEM_TOKEN_CONSUME_PROCESS_MODULE_H
#define NFI_ITEM_TOKEN_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIItemTokenConsumeProcessModule
    : public NFIModule
{
public:


    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;


    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID) = 0;

};

#endif
