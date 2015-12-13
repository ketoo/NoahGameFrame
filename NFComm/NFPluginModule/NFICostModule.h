// -------------------------------------------------------------------------
//    @FileName         £º    NFICostModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2015/05/18
//    @Module           £º    NFICostModule
//
// -------------------------------------------------------------------------

#ifndef __NFI_COST_MODULE_H_
#define __NFI_COST_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"


class NFICostModule : public NFILogicModule
{

public:
    virtual bool Cost(const NFGUID& self, const std::string& strCostID) = 0;
    virtual bool Cost(const NFGUID& self, const std::list<std::string>& xCostList) = 0;
};

#endif // !__NFI_COST_MODULE_H_