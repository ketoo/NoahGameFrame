// -------------------------------------------------------------------------
//    @FileName         :    NFCUUIDModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-4-9
//    @Module           :    NFCUUIDModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_UUID_MODULE_H_
#define _NFC_UUID_MODULE_H_

#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/crc.hpp>

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

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual int64_t CreateGUID();
    virtual int64_t CreateGUID(const std::string& strName);

private:
    UUIDModule::UUID* m_pUUID;
};

#endif // !_NFC_UUID_MODULE_H_