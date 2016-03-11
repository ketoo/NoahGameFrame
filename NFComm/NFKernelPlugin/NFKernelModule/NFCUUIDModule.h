// -------------------------------------------------------------------------
//    @FileName         :    NFCUUIDModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-4-9
//    @Module           :    NFCUUIDModule
//
// -------------------------------------------------------------------------

#ifndef NFC_UUID_MODULE_H
#define NFC_UUID_MODULE_H

#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

namespace UUIDModule
{
extern uint64_t get_time();
class UUID;
}

class NFCUUIDModule
    : public NFIUUIDModule
{
public:
    NFCUUIDModule(NFIPluginManager* p);
    virtual ~NFCUUIDModule() {}

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual NFGUID CreateGUID();

    virtual NFINT64 GetIdentID();
    virtual void SetIdentID(NFINT64 nID);
private:

    NFINT64 mnIdent;
    UUIDModule::UUID* m_pUUID;
    NFIKernelModule* m_pKernelModule;
};

#endif // !_NFC_UUID_MODULE_H