// -------------------------------------------------------------------------
//    @FileName      	:   NFIItemConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-02-16
//    @Module           :   NFIItemConsumeManagerModule

// -------------------------------------------------------------------------

#ifndef NFI_ITEM_CONSUME_MANAGER_MODULE_H
#define NFI_ITEM_CONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFIItemConsumeProcessModule.h"

class NFIItemConsumeManagerModule
    : public NFIModule
{
public:
    virtual bool ResgisterConsumeModule(const int nModuleType, NF_SHARE_PTR<NFIItemConsumeProcessModule> pModule) = 0;
	virtual NF_SHARE_PTR<NFIItemConsumeProcessModule> GetConsumeModule(const int nModuleType) = 0;

};

#endif
