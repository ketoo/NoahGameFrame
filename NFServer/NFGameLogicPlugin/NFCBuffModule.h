// -------------------------------------------------------------------------
//    @FileName      :    NFCBuffModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCBuffModule
//
// -------------------------------------------------------------------------

#ifndef NFC_BUFF_MODULE_H
#define NFC_BUFF_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIBuffModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCBuffModule
    : public NFIBuffModule
{
public:
    enum RUNTIME_BUFF_INFO
    {
        RTBI_BUFFCONFIG,//string
        RTBI_RELEASER,//object
        RTBI_INTERVALTIME,//float
        RTBI_REMAINTIME,//int
        RTBI_EFFECTVALUE,//string
    };

    NFCBuffModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCBuffModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();


    
    virtual bool AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent );

    
    virtual bool RemoveBuff( const NFGUID& self, const std::string& strConfig );

    
    virtual bool RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType );

    
    virtual bool SelfFree( const NFGUID& self );

    
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    
    virtual bool HasBuff( const NFGUID& self, const std::string& strConfig );

    
    virtual bool Execute( const NFGUID& self, float fPassTime );

    
    virtual int Attack( const NFGUID& self, const NFGUID& other );

    
    virtual int OnDead( const NFGUID& self );

protected:
    
    virtual bool ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    
    virtual int ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    
    virtual int ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    
    virtual int ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    
    virtual int ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    
    virtual int ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    
    virtual std::string GetGroupBuffIndex( const NFGUID& self, int nGroupID );

    
    virtual void SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;

    char* mstrRunTimeEffectTable;
};


#endif
