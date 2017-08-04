// -------------------------------------------------------------------------
//    @FileName			:    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LEVEL_MODULE_H
#define NFC_LEVEL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFCLevelModule
    : public NFILevelModule
{
public:
    NFCLevelModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCLevelModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int AddExp(const NFGUID& self, const int64_t nExp);

private:
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
};

#endif
