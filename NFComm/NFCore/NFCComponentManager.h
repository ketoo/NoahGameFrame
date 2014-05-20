// -------------------------------------------------------------------------
//    @FileName         :    NFCComponentManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponentManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_COMPONENT_MANAGER_H_
#define _NFC_COMPONENT_MANAGER_H_

#include "NFCComponent.h"
#include "NFIComponentManager.h"

class NFCComponentManager : public NFIComponentManager
{
public:
    NFCComponentManager(const NFIDENTID& self)
    {
        mSelf = self;
    }
private:
    NFIDENTID mSelf;
};

#endif