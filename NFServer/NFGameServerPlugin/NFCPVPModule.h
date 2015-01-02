// -------------------------------------------------------------------------
//    @FileName      :    NFCPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PVP_MODULE_H_
#define _NFC_PVP_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

	virtual bool MatchPVPObject(const NFIDENTID& self) = 0;

	virtual bool StartPVPWar(const NFIDENTID& self) = 0;

	virtual bool ExitPVPWar(const NFIDENTID& self) = 0;

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;

};


#endif
