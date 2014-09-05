// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGameServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_MODULE_H_
#define _NFC_GAMESERVER_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCGameServerModule
    : public NFIGameServerModule
{
public:
    NFCGameServerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGameServerModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIDataNoSqlModule* m_pNoSqlModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
