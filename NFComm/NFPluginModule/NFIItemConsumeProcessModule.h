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
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

class NFIItemConsumeProcessModule
    : public NFIModule
{
public:

	// > 0, error code
    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID) = 0;

	//> 0, error code
    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID) = 0;

};

#endif
