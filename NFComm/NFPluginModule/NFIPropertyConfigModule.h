// -------------------------------------------------------------------------
//    @FileName      :    NFIPropertyConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFIPropertyConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_CONFIG_MODULE_H
#define _NFI_PROPERTY_CONFIG_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

enum NFJobType
{
	NJT_1,
	NJT_2,
	NJT_3,
	NJT_4,
	NJT_MAX,
};

class NFIPropertyConfigModule
    : public NFILogicModule
{
public:

    virtual int CalculateBaseValue(const NFJobType jobType,  const int nLevel, const std::string& strProperty) = 0;
    virtual bool LegalLevel( const NFJobType nJob, const int nLevel ) = 0;
};


#endif
