// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCHeroModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HERO_MODULE_MODULE_H
#define NFC_HERO_MODULE_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCHeroModule
    : public NFIHeroModule
{
public:
    NFCHeroModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCHeroModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
