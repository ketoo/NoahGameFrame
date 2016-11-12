// -------------------------------------------------------------------------
//    @FileName			:    NFCWorldLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCWorldLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLDLOGIC_MODULE_H
#define NFC_WORLDLOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"

class NFCWorldLogicModule
    : public NFIWorldLogicModule
{
public:
    NFCWorldLogicModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

protected:
    NFIKernelModule* m_pKernelModule;
private:
};

#endif