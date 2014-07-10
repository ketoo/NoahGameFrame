// -------------------------------------------------------------------------
//    @FileName      :    NFCGameLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGameLogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_GAMELOGIC_MODULE_H_
#define _NFC_GAMELOGIC_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCGameLogicModule
    : public NFIGameLogicModule
{
public:
    NFCGameLogicModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGameLogicModule() {};

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
