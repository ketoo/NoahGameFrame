// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCPropertyModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPropertyModule.h"
#include "NFComm/NFCore/NFTimer.h"

bool NFCPropertyModule::Init()
{
    mstrCommPropertyName = "CommPropertyValue";

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    m_pPropertyConfigModule = dynamic_cast<NFIPropertyConfigModule*>( pPluginManager->FindModule( "NFCPropertyConfigModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pPropertyConfigModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCPropertyModule::OnObjectClassEvent );

    return true;
}


bool NFCPropertyModule::Shut()
{
    return true;
}

bool NFCPropertyModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCPropertyModule::AfterInit()
{
    return true;
}

int NFCPropertyModule::GetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        return m_pKernelModule->QueryRecordInt( self, mstrCommPropertyName, eGroupType, "strPropertyName" );
    }

    return m_pKernelModule->QueryPropertyInt( self, mstrCommPropertyName );
}

int NFCPropertyModule::SetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt( eGroupType, strPropertyName, nValue );
            }
        }

        //return m_pKernelModule->SetRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol, nValue );
    }

    //动态表中没有，则设置到最终值
    m_pKernelModule->SetPropertyInt( self, strPropertyName, nValue );

    return 0;
}


int NFCPropertyModule::AddPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->QueryInt(eGroupType, strPropertyName );

                return pRecord->SetInt( eGroupType, strPropertyName, nPropertyValue + nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::SubPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->QueryInt(eGroupType, strPropertyName );

                return pRecord->SetInt( eGroupType, strPropertyName, nPropertyValue - nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    RefreshBaseProperty( self );

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int NFCPropertyModule::OnRecordPropertyEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    //计算总值
 
    int nAllValue = 0;
    NFIRecord* pRecord = m_pKernelModule->FindRecord(self, mstrCommPropertyName);
    for ( int i = 0; i < ( int )( NFPropertyGroup::NPG_ALL ); i++ )
    {
        if ( i < pRecord->GetRows() )
        {
            int nValue = pRecord->QueryInt( i, nCol );
            nAllValue += nValue;
        }
    }

    m_pKernelModule->SetPropertyInt( self, pRecord->GetColTag(nCol), nAllValue );

    return 0;
}

int NFCPropertyModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            m_pKernelModule->AddPropertyCallBack( self, "Level", this, &NFCPropertyModule::OnObjectLevelEvent );
            
            // TODO:一级属性回调
            m_pKernelModule->AddRecordCallBack( self, mstrCommPropertyName, this, &NFCPropertyModule::OnRecordPropertyEvent );
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            int nOnlineCount = m_pKernelModule->QueryPropertyInt( self, "OnlineCount" );
            if ( nOnlineCount <= 0 && m_pKernelModule->QueryPropertyInt( self, "SceneID" ) > 0 )
            {
                //第一次出生，设置基础属性
                m_pKernelModule->SetPropertyInt( self, "Level", 1 );
            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {

        }
    }

    return 0;
}

int NFCPropertyModule::RefreshBaseProperty( const NFIDENTID& self )
{
    //初始属性+等级属性(职业决定)
    NFJobType eJobType = ( NFJobType )m_pKernelModule->QueryPropertyInt( self, "Job" );
    int nLevel = m_pKernelModule->QueryPropertyInt( self, "Level" );

    
    NFIRecord* pRecord = m_pKernelModule->FindRecord(self, mstrCommPropertyName);
    for ( int i = 0; i < pRecord->GetCols(); ++i )
    {
        const std::string& strColTag = pRecord->GetColTag(i);
        int nValue = m_pPropertyConfigModule->CalculateBaseValue( eJobType, nLevel, strColTag );
        SetPropertyValue( self, strColTag, NFPropertyGroup::NPG_JOBLEVEL, nValue );
    }

    return 1;
}

bool NFCPropertyModule::FullHPMP( const NFIDENTID& self )
{
    int nMaxHP = m_pKernelModule->QueryPropertyInt( self, "MAXHP" );
    if ( nMaxHP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "HP", nMaxHP );
    }

    int nMaxMP = m_pKernelModule->QueryPropertyInt( self, "MAXMP" );
    if ( nMaxMP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "MP", nMaxMP );
    }

    return true;
}

bool NFCPropertyModule::AddHP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "HP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXHP" );

    if ( nCurValue > 0 )
    {
        nCurValue += nValue;
        if ( nCurValue > nMaxValue )
        {
            nCurValue = nMaxValue;
        }
        m_pKernelModule->SetPropertyInt( self, "HP", nCurValue );
    }

    return true;
}

bool NFCPropertyModule::AddMP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "MP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXMP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeHP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "HP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "HP", nCurValue );

        return true;
    }

    return false;
}

bool NFCPropertyModule::ConsumeMP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "MP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

        return true;
    }

    return false;
}

bool NFCPropertyModule::ConsumeSP( const NFIDENTID& self, int nValue )
{
    int nCSP = m_pKernelModule->QueryPropertyInt( self, "SP" );
    if ( ( nCSP > 0 ) && ( nCSP - nValue >= 0 ) )
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt( self, "SP", nCSP );

        return true;
    }

    return false;
}

bool NFCPropertyModule::FullSP( const NFIDENTID& self )
{
    int nMAXCSP = m_pKernelModule->QueryPropertyInt( self, "MAXSP" );
    if ( nMAXCSP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "SP", nMAXCSP );

        return true;
    }

    return false;
}

bool NFCPropertyModule::AddSP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "SP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXSP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, "SP", nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeMoney( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Money" );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

        return true;
    }    

    return false;
}

bool NFCPropertyModule::AddMoney( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Money" );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

    return false;
}

int NFCPropertyModule::OnReqModifyData( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    if (var.GetCount() != 6
        || !var.TypeEx(VARIANT_TYPE::VTYPE_OBJECT, VARIANT_TYPE::VTYPE_OBJECT, VARIANT_TYPE::VTYPE_STRING, 
        VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return 1;
    }

    NFIDENTID identSelf = var.ObjectVal(0);
    NFIDENTID identOther = var.ObjectVal(1);
    const std::string& strName = var.StringVal(2);
    int nType = var.IntVal(3);
    int nValue = var.IntVal(4);
    const std::string& strKey = var.StringVal(5);

    if (identSelf == self
        && identSelf == identOther)
    {
        int nCurValue = m_pKernelModule->QueryPropertyInt( self, strName );
        if (0 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nValue);
        }
        else if (1 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nValue + nCurValue);
        }
        else if (2 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nCurValue - nValue );
        }
    }

    return 0;
}