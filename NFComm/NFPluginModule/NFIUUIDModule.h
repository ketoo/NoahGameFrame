// -------------------------------------------------------------------------
//    @FileName         :    NFIUUIDModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-4-9
//    @Module           :    NFIUUIDModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_UUID_MODULE_H_
#define _NFI_UUID_MODULE_H_

#include "NFILogicModule.h"

class NFIUUIDModule
    : public NFILogicModule
{
public:
    virtual int64_t CreateGUID() = 0;
    virtual int64_t CreateGUID(const std::string& strName) = 0;

};

#endif // !_NFI_UUID_MODULE_H_