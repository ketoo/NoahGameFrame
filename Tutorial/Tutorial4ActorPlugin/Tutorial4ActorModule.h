// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld4Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld4Actor
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD4_H
#define NFC_HELLO_WORLD4_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class HelloWorld4ActorModule
    : public NFILogicModule
{
public:
    HelloWorld4ActorModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);
	int OnSyncEvent(const NFGUID& self, const int nActorID, const int event, const std::string& arg);

protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
