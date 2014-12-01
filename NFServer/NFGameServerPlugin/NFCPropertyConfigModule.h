// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTY_CONFIG_MODULE_H_
#define _NFC_PROPERTY_CONFIG_MODULE_H_

#include "NFComm/Config/NFConfig.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCPropertyConfigModule
    : public NFIPropertyConfigModule
{
public:
    NFCPropertyConfigModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCPropertyConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual int CalculateBaseValue( const NFJobType nJob, const int nLevel, const std::string& strProperty );
    virtual bool LegalLevel( const NFJobType nJob, const int nLevel );

protected:
    void Load();
private:
    //µÈ¼¶->EffectData
    NFMapEx<int, std::string> mhtCoefficienData[( int )NFJobType::NJT_MAX];

    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
};


#endif
