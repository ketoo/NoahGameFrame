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
#include "NFComm/NFPluginModule/NFILevelConfigModule.h"

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
    virtual int GetPropertyValue( const NFIDENTID& self, const NFIPropertyConfigModule::NFRumTimeColIndex nCol, const NFPropertyGroup eGroupType );
    virtual int SetPropertyValue( const NFIDENTID& self, const NFIPropertyConfigModule::NFRumTimeColIndex nCol, const NFPropertyGroup eGroupType, const int nValue );

    virtual int AddPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue );
    virtual int SubPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue );

    virtual int FullHPMP( const NFIDENTID& self );

    virtual int AddHP( const NFIDENTID& self, int nValue );
    virtual int ConsumeHP( const NFIDENTID& self, int nValue );

    virtual int AddMP( const NFIDENTID& self, int nValue );
    virtual int ConsumeMP( const NFIDENTID& self, int nValue );    

    virtual int ConsumeSP( const NFIDENTID& self, int nValue );
    virtual int FullSP( const NFIDENTID& self );
    virtual int AddSP( const NFIDENTID& self, int nValue );

    virtual int ConsumeVP( const NFIDENTID& self, int nValue );
    virtual int FullVP( const NFIDENTID& self );
    virtual int RestoreVP( const NFIDENTID& self );
    virtual int AddVP( const NFIDENTID& self, int nValue );

    virtual int ConsumeActivityVP( const NFIDENTID& self, int nValue );
    virtual int FullActivityVP( const NFIDENTID& self );
    virtual int AddActivityVP( const NFIDENTID& self, int nValue );

    virtual int ConsumeMoney( const NFIDENTID& self, int nValue );
    virtual int AddMoney( const NFIDENTID& self, int nValue );

    virtual int ConsumeYBP(const NFIDENTID& self, int nValue);

    virtual int ConsumeSoulStone( const NFIDENTID& self, int nValue );
    virtual int AddSoulStone( const NFIDENTID& self, int nValue );

    virtual int ConsumeEssence( const NFIDENTID& self, int nValue );
    virtual int AddEssence( const NFIDENTID& self, int nValue );

	virtual int AddHonour(const NFIDENTID& self, int nValue);
	virtual int ConsumeHonour(const NFIDENTID& self, int nValue);
protected:


protected:
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );

    int OnObjectPropertyEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnRecordPropertyEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnReqModifyData( const NFIDENTID& self, const int nEventID, const NFIValueList& var );

    int OnHPRecoverHeart( const NFIDENTID& self, const NFIValueList& var );
    int OnMPRecoverHeart( const NFIDENTID& self, const NFIValueList& var );
    int OnSPRecoverHeart( const NFIDENTID& self, const NFIValueList& var );
    int OnVPRecoverHeart( const NFIDENTID& self, const NFIValueList& var );
    int OnActivityVPRecoverHeart( const NFIDENTID& self, const NFIValueList& var );
private:
    //NFConfig mConfig;
private:
    char* mstrCommPropertyName;

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILevelConfigModule* m_pLevelConfigModule;

    NFMap<std::string, int>  mPropertyColList; //主要是运行时表property->col那里
    NFMap<int, std::string>  mColPropertyList; //主要是运行时表col->property


    //     char* mstrPropertyName[(int)NFPropertyColIndex::PROPERTY_MAX];
    //char* mstrRumTimeTableIndex[(int)NFRumTimeColIndex::NFRTC_END];
};


#endif
