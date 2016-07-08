// -------------------------------------------------------------------------
//    @FileName         :    NFIUUIDModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-4-9
//    @Module           :    NFIUUIDModule
//
// -------------------------------------------------------------------------

#ifndef NFI_UUID_MODULE_H
#define NFI_UUID_MODULE_H

#include "NFIModule.h"

class NFIUUIDModule
    : public NFIModule
{
public:
    virtual NFGUID CreateGUID() = 0;
    virtual NFINT64 GetIdentID() = 0;
    virtual void SetIdentID(NFINT64 nID) = 0;
};

#endif // !NFI_UUID_MODULE_H