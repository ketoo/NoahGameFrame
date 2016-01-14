// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGINLOGIC_MODULE_H
#define NFC_LOGINLOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"

class NFCLoginLogicModule
    : public NFILoginLogicModule
{
public:
    NFCLoginLogicModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();


protected:

    // client event
    int OnLoginEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnDisconnectEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

protected:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIClusterModule* m_pClusterSqlModule;

private:
};

#endif