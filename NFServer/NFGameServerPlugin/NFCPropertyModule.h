// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCPropertyModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_MODULE_H
#define NFC_PROPERTY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFILevelModule.h"

class NFCPropertyModule : public NFIPropertyModule
{
public:
    NFCPropertyModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int RefreshBaseProperty(const NFGUID& self);

    virtual int GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType);
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue);

    virtual int AddPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue);
    virtual int SubPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue);

    virtual bool FullHPMP(const NFGUID& self);
    virtual bool AddHP(const NFGUID& self, const NFINT64& nValue);
    virtual bool ConsumeHP(const NFGUID& self, const NFINT64& nValue);
    virtual bool EnoughHP(const NFGUID& self, const NFINT64& nValue);

    virtual bool AddMP(const NFGUID& self, const NFINT64& nValue);
    virtual bool ConsumeMP(const NFGUID& self, const NFINT64& nValue);
    virtual bool EnoughMP(const NFGUID& self, const NFINT64& nValue);

    virtual bool FullSP(const NFGUID& self);
    virtual bool AddSP(const NFGUID& self, const NFINT64& nValue);
    virtual bool ConsumeSP(const NFGUID& self, const NFINT64& nValue);
    virtual bool EnoughSP(const NFGUID& self, const NFINT64& nValue);

    virtual bool AddMoney(const NFGUID& self, const NFINT64& nValue);
    virtual bool ConsumeMoney(const NFGUID& self, const NFINT64& nValue);
    virtual bool EnoughMoney(const NFGUID& self, const NFINT64& nValue);

    virtual bool AddDiamond(const NFGUID& self, const NFINT64& nValue);
    virtual bool ConsumeDiamond(const NFGUID& self, const NFINT64& nValue);
    virtual bool EnoughDiamond(const NFGUID& self, const NFINT64& nValue);

protected:
    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int OnObjectLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    int OnRecordPropertyEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

private:
    NFIKernelModule* m_pKernelModule;
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILevelModule* m_pLevelModule;
};


#endif
