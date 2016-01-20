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
    virtual int OnReqRankList(const NFGUID& self, const int nRankType, const int nStart, const int nEnd) = 0;
    
    virtual void LoadRankList() = 0;
    virtual void ProcessRankValueUpdate(const NFGUID& self, const int nRankType, const int nOperateType, const NFINT64 nValue) = 0;
};

#endif // !NFI_RANK_MODULE_H