// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_DATABASENET_CLIENT_MODULE_H_
#define _NFC_DATABASENET_CLIENT_MODULE_H_

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIDataBaseNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIMysqlConnectMgrModule.h"

class NFCDataBaseNet_ClientModule : public NFIDataBaseNet_ClientModule
{
public:
    NFCDataBaseNet_ClientModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

private:
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIMysqlConnectMgrModule* m_pMysqlConnectMgrModule;
};

#endif