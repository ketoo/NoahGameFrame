// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_DATABASENET_CLIENT_MODULE_H
#define NFC_DATABASENET_CLIENT_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIDataBaseNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIMysqlConnectMgrModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"

class NFCDataBaseNet_ClientModule : public NFIDataBaseNet_ClientModule
{
public:
    NFCDataBaseNet_ClientModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

private:
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIClusterModule* m_pMysqlClusterModule;
};

#endif