// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld6.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld6
//
// -------------------------------------------------------------------------

#ifndef _NFC_HELLO_WORLD3_H_
#define _NFC_HELLO_WORLD3_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

class HelloWorld6Module
    : public NFILogicModule
{
public:
    HelloWorld6Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILuaScriptModule* m_pLuaScriptModule;
};

#endif
