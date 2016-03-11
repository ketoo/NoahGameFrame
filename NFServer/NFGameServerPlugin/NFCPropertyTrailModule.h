// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyTrailModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyTrailModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_TRAIL_MODULE_H
#define NFC_PROPERTY_TRAIL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
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

    int OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    int OnObjectRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

private:

    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
};


#endif
