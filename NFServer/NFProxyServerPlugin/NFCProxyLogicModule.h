// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROXYLOGIC_MODULE_H
#define NFC_PROXYLOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
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
    virtual bool Execute();

    virtual bool AfterInit();


protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif