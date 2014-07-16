// -------------------------------------------------------------------------
//    @FileName      :    NFIPropertyConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIPropertyConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_CONFIG_MODULE_H_
#define _NFI_PROPERTY_CONFIG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIPropertyConfigModule
    : public NFILogicModule
{
public:

    virtual int CalculateBaseValue(const NFJobType jobType,  const int nLevel, const std::string& strProperty) = 0;
};


#endif
