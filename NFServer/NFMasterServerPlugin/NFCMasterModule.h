// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFC_MASTER_MODULE_H
#define NFC_MASTER_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIMasterModule.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"

class NFCMasterModule
    : public NFIMasterModule
{

public:
    NFCMasterModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
protected:


private:

    NFIKernelModule* m_pKernelModule;
};

#endif