// -------------------------------------------------------------------------
//    @FileName      :    NFCGameLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGameLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GAMELOGIC_MODULE_H
#define NFC_GAMELOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

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
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:


protected:
    NFIClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
