// -------------------------------------------------------------------------
//    @FileName			:    NFCPropertyTrailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyTrailModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_TRAIL_MODULE_H
#define NFC_PROPERTY_TRAIL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPropertyTrailModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCPropertyTrailModule
    : public NFIPropertyTrailModule
{
public:
    NFCPropertyTrailModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCPropertyTrailModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void StartTrail(const NFGUID self);
    virtual void EndTrail(const NFGUID self);

protected:

    int LogObjectData(const NFGUID& self);
    int TrailObjectData(const NFGUID& self);

    int OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

    int OnObjectRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

private:

    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
};


#endif
