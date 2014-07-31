// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_LOGINLOGIC_MODULE_H_
#define _NFC_LOGINLOGIC_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFIPlatformDataModule.h"

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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();


protected:

    // client event
    int OnLoginEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

    int OnDisconnectEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

protected:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIPlatformDataModule* m_pNoSqlModule;
    NFIKernelModule* m_pKernelModule;

private:
};

#endif