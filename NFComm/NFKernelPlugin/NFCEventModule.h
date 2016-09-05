// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENT_MODULE_H
#define NFC_EVENT_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFCEventModule
    : public NFIEventModule
{
public:
    NFCEventModule(NFIPluginManager* p)
    {
        //pPluginManager = p;
    }


    virtual ~NFCEventModule()
    {
    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();



protected:
private:
};

#endif