// -------------------------------------------------------------------------
//    @FileName         :   NFIRankModule.h
//    @Author           :   NickYang
//    @Date             :   2012/12/15
//    @Module           :   NFIRankModule
//
// -------------------------------------------------------------------------

#ifndef NFI_RANK_MODULE_H
#define NFI_RANK_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFIRankModule : public NFILogicModule
{
public:
    virtual void ProcessRankValueUpdate(const NFGUID& self, const int nRankType, const int nOperateType, const NFINT64 nValue) = 0;
};

#endif // !NFI_RANK_MODULE_H