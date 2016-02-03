// -------------------------------------------------------------------------
//    @FileName      :    NFCGameLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGameLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GAMELOGIC_MODULE_H
#define NFC_GAMELOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
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
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
