// -------------------------------------------------------------------------
//    @FileName         £º    NFCMasterModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFCMasterModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_MASTER_MODULE_H_
#define _NFC_MASTER_MODULE_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIMasterModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
protected:


private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
};

#endif