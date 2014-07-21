// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyConfigModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyConfigModule
//
// -------------------------------------------------------------------------

#include "NFCPropertyConfigModule.h"
#include "NFComm\NFPluginModule\NFIPluginManager.h"

bool NFCPropertyConfigModule::Init()
{

    return true;
}


bool NFCPropertyConfigModule::Shut()
{
    return true;
}

bool NFCPropertyConfigModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCPropertyConfigModule::AfterInit()
{
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);

    Load();

    return true;
}

int NFCPropertyConfigModule::CalculateBaseValue( const NFJobType nJob, const int nLevel, const std::string& strProperty )
{
    if ( nJob >= 0 && nJob < NFJobType::NJT_MAX )
    {
        std::string* pstrEffectData = mhtCoefficienData[nJob].GetElement( nLevel );
        if ( pstrEffectData )
        {
            return m_pElementInfoModule->QueryPropertyInt(*pstrEffectData, strProperty);
        }
    }

    return 0;
}

void NFCPropertyConfigModule::Load()
{

    NFILogicClass* pLogicClass = m_pLogicClassModule->GetElement("InitProperty");
    if (pLogicClass)
    {
        NFList<std::string>& xList = pLogicClass->GetConfigNameList();
        std::string strData;
        bool bRet = xList.First(strData);
        while (bRet)
        {
            NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager(strData);
            if (pPropertyManager)
            {
                int nJob = m_pElementInfoModule->QueryPropertyInt(strData, "Job");
                int nLevel = m_pElementInfoModule->QueryPropertyInt(strData, "Level");
                std::string strEffectData = m_pElementInfoModule->QueryPropertyString(strData, "EffectData");
                mhtCoefficienData[nJob].AddElement( nLevel, new std::string(strEffectData) );
            }

            bRet = xList.Next(strData);
        }
    }
}

bool NFCPropertyConfigModule::LegalLevel( const NFJobType nJob, const int nLevel )
{
    std::string* pstrEffectData = mhtCoefficienData[nJob].GetElement( nLevel );
    if (pstrEffectData)
    {
        return true;
    }

    return false;
}