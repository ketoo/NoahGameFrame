// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld6.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld6
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD6_H
#define NFC_HELLO_WORLD6_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
    NFILuaScriptModule* m_pLuaScriptModule;
};

#endif
