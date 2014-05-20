// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROXYLOGIC_MODULE_H_
#define _NFC_PROXYLOGIC_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
//#include "NFComm/NFPluginModule/NFIShareMemoryModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"

class NFCProxyLogicModule
    : public NFIProxyLogicModule
{
public:
    NFCProxyLogicModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();


protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    //NFIShareMemoryModule* m_pShareMemoryModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif