// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTY_MODULE_H_
#define _NFC_PROPERTY_MODULE_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCPropertyModule
    : public NFIPropertyModule
{
public:
    NFCPropertyModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual int RefreshBaseProperty( const NFIDENTID& self );

    virtual int GetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType );
    virtual int SetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue );

    virtual int AddPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue );
    virtual int SubPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue );

    virtual bool FullHPMP(const NFIDENTID& self);
    virtual bool AddHP(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool ConsumeHP(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool EnoughHP(const NFIDENTID& self, const NFINT64& nValue);

    virtual bool AddMP(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool ConsumeMP(const NFIDENTID& self, const NFINT64& nValue);    
    virtual bool EnoughMP(const NFIDENTID& self, const NFINT64& nValue);

    virtual bool FullSP(const NFIDENTID& self);
    virtual bool AddSP(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool ConsumeSP(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool EnoughSP(const NFIDENTID& self, const NFINT64& nValue);

    virtual bool AddMoney(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool ConsumeMoney(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool EnoughMoney(const NFIDENTID& self, const NFINT64& nValue);

    virtual bool AddDiamond(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool ConsumeDiamond(const NFIDENTID& self, const NFINT64& nValue);
    virtual bool EnoughDiamond(const NFIDENTID& self, const NFINT64& nValue);

protected:
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    int OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );

    int OnRecordPropertyEvent( const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar);
private:
    //NFConfig mConfig;
private:
    char* mstrCommPropertyName;

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
};


#endif
