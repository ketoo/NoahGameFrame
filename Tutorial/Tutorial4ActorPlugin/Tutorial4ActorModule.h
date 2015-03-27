// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld4Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld4Actor
//
// -------------------------------------------------------------------------

#ifndef _NFC_HELLO_WORLD4_H_
#define _NFC_HELLO_WORLD4_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

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

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const NFIDENTID& self, const int event, const NFIDataList& arg);

protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
