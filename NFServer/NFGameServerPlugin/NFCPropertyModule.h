// -------------------------------------------------------------------------
//    @FileName			:    NFCPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCPropertyModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_MODULE_H
#define NFC_PROPERTY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
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

    virtual int64_t GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType);
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue);

	virtual bool CalculatePropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue, const bool bPositive = false);

    virtual bool FullHPMP(const NFGUID& self);
    virtual bool AddHP(const NFGUID& self, const int nValue);
    virtual bool ConsumeHP(const NFGUID& self, const int nValue);
	virtual bool EnoughHP(const NFGUID& self, const int nValue);
	virtual bool DamageHP(const NFGUID& self, const int nValue);

    virtual bool AddMP(const NFGUID& self, const int nValue);
    virtual bool ConsumeMP(const NFGUID& self, const int nValue);
	virtual bool EnoughMP(const NFGUID& self, const int nValue);
	virtual bool DamageMP(const NFGUID& self, const int nValue);

    virtual bool FullSP(const NFGUID& self);
    virtual bool AddSP(const NFGUID& self, const int nValue);
    virtual bool ConsumeSP(const NFGUID& self, const int nValue);
    virtual bool EnoughSP(const NFGUID& self, const int nValue);

    virtual bool AddGold(const NFGUID& self, const int64_t nValue);
    virtual bool ConsumeGold(const NFGUID& self, const int64_t nValue);
    virtual bool EnoughGold(const NFGUID& self, const int64_t nValue);

    virtual bool AddDiamond(const NFGUID& self, const int nValue);
    virtual bool ConsumeDiamond(const NFGUID& self, const int nValue);
    virtual bool EnoughDiamond(const NFGUID& self, const int nValue);

protected:
    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    int OnObjectLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

    int OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

private:
    NFIKernelModule* m_pKernelModule;
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFILevelModule* m_pLevelModule;
};


#endif
