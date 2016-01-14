// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PVP_MODULE_H
#define NFC_PVP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCPVPModule
    : public NFIPVPModule
{
public:
    NFCPVPModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFCPVPModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool MatchPVPObject(const NFGUID& self);

	virtual bool StartPVPWar(const NFGUID& self);

	virtual bool ExitPVPWar(const NFGUID& self);

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;

private:
    NFList<NFGUID> mxPVPList; // 报名的队列
};


#endif
