// -------------------------------------------------------------------------
//    @FileName			:    NFCMapModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCMapModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MAP_MODULE_H
#define NFC_MAP_MODULE_H

#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCMapModule
    : public NFICommonConfigModule
{
public:

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();
};


#endif
