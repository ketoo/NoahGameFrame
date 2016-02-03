// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld5.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld5
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD5_H
#define NFC_HELLO_WORLD5_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class HelloWorld5Module
    : public NFILogicModule
{
public:
    HelloWorld5Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

    virtual int OnSyncEvent(const NFGUID& self, const int nActorID, const int event, const std::string& arg);
protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
