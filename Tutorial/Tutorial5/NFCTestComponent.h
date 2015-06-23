// -------------------------------------------------------------------------
//    @FileName      :    NFCTestComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFCTestComponent
//
// -------------------------------------------------------------------------

#ifndef _NFC_TEST_COMPONENT_H_
#define _NFC_TEST_COMPONENT_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCTestComponent
    : public NFIComponent
{
public:
    NFCTestComponent(NFIPluginManager* p)
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
};

#endif
