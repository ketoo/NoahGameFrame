// -------------------------------------------------------------------------
//    @FileName         £º    NFISLGModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2014-09-11
//    @Module           £º    NFISLGModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_MODULE_H_
#define _NFC_GAMESERVER_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISLGModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCSLGModule : public NFISLGModule
{
public:
    NFCSLGModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCSLGModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:


protected:
    NFIDataNoSqlModule* m_pNoSqlModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
