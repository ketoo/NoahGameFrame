// -------------------------------------------------------------------------
//    @FileName         £º    NFIUrlCodeModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIUrlCodeModule
//
// -------------------------------------------------------------------------

#ifndef NFI_URLCODE_MODULE_H
#define NFI_URLCODE_MODULE_H

#include "NFILogicModule.h"

class NFIUrlCodeModule
    : public NFILogicModule
{
public:
    virtual std::string EncodeURIValue(const std::string& strSource) = 0;
    virtual std::string DecodeURIValue(const std::string& strSource) = 0;
};

#endif