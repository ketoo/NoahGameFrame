// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTY_MODULE_H_
#define _NFC_PROPERTY_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"

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

    virtual bool FullHPMP( const NFIDENTID& self );
    virtual bool AddHP( const NFIDENTID& self, int nValue );
    virtual bool ConsumeHP( const NFIDENTID& self, int nValue );

    virtual bool AddMP( const NFIDENTID& self, int nValue );
    virtual bool ConsumeMP( const NFIDENTID& self, int nValue );    

    virtual bool ConsumeSP( const NFIDENTID& self, int nValue );
    virtual bool FullSP( const NFIDENTID& self );
    virtual bool AddSP( const NFIDENTID& self, int nValue );

    virtual bool ConsumeMoney( const NFIDENTID& self, int nValue );
    virtual bool AddMoney( const NFIDENTID& self, int nValue );

protected:


protected:
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );

    int OnObjectPropertyEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnRecordPropertyEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnReqModifyData( const NFIDENTID& self, const int nEventID, const NFIValueList& var );

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
