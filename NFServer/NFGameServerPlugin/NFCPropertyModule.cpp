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

int NFCPropertyModule::GetPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        return m_pKernelModule->GetRecordInt( self, mstrCommPropertyName, eGroupType, strPropertyName );
    }

    return m_pKernelModule->GetPropertyInt( self, mstrCommPropertyName );
}

int NFCPropertyModule::SetPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject.get())
        {
            NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord.get())
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt( eGroupType, strPropertyName, nValue );
            }
        }

        //return m_pKernelModule->SetRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol, nValue );
    }

    //��̬����û�У������õ�����ֵ
    m_pKernelModule->SetPropertyInt( self, strPropertyName, nValue );

    return 0;
}


int NFCPropertyModule::AddPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject.get())
        {
            NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord.get())
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName );

                return pRecord->SetInt( eGroupType, strPropertyName, nPropertyValue + nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::SubPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject.get())
        {
            NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord.get())
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName );

                return pRecord->SetInt( eGroupType, strPropertyName, nPropertyValue - nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::OnObjectLevelEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
    RefreshBaseProperty( self );

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int NFCPropertyModule::OnRecordPropertyEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar )
{
    //������ֵ
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

    int nAllValue = 0;
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, mstrCommPropertyName);
    for ( int i = 0; i < ( int )( NFPropertyGroup::NPG_ALL ); i++ )
    {
        if ( i < pRecord->GetRows() )
        {
            int nValue = pRecord->GetInt( i, nCol );
            nAllValue += nValue;
        }
    }

    m_pKernelModule->SetPropertyInt( self, pRecord->GetColTag(nCol), nAllValue );

    return 0;
}

int NFCPropertyModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, mstrCommPropertyName);
            if (pRecord.get())
            {
                for(int i =0; i< NPG_ALL; i++)
                {
                    pRecord->AddRow(-1);
                }
            }

            m_pKernelModule->AddPropertyCallBack( self, "Level", this, &NFCPropertyModule::OnObjectLevelEvent );

            // TODO:һ�����Իص�
            m_pKernelModule->AddRecordCallBack( self, mstrCommPropertyName, this, &NFCPropertyModule::OnRecordPropertyEvent );


        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt( self, "OnlineCount" );
            if ( nOnlineCount <= 0 && m_pKernelModule->GetPropertyInt( self, "SceneID" ) > 0 )
            {
                //��һ�γ��������û�������
                m_pKernelModule->SetPropertyInt( self, "Level", 1 );
            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {

        }
    }

    return 0;
}

int NFCPropertyModule::RefreshBaseProperty( const NFGUID& self )
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, mstrCommPropertyName);
    if (!pRecord.get())
    {
        return 1;
    }

    //��ʼ����+�ȼ�����(ְҵ����)
    NFJobType eJobType = ( NFJobType )m_pKernelModule->GetPropertyInt( self, "Job" );
    int nLevel = m_pKernelModule->GetPropertyInt( self, "Level" );

    for ( int i = 0; i < pRecord->GetCols(); ++i )
    {
        const std::string& strColTag = pRecord->GetColTag(i);
        int nValue = m_pPropertyConfigModule->CalculateBaseValue( eJobType, nLevel, strColTag );
        SetPropertyValue( self, strColTag, NFPropertyGroup::NPG_JOBLEVEL, nValue );
    }

    return 1;
}

bool NFCPropertyModule::FullHPMP(const NFGUID& self)
{
    NFINT64 nMaxHP = m_pKernelModule->GetPropertyInt(self, "MAXHP");
    if (nMaxHP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, "HP", nMaxHP);
    }

    NFINT64 nMaxMP = m_pKernelModule->GetPropertyInt(self, "MAXMP");
    if (nMaxMP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, "MP", nMaxMP);
    }

    return true;
}

bool NFCPropertyModule::AddHP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "HP");
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, "MAXHP");

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

bool NFCPropertyModule::EnoughHP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "HP");
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::ConsumeHP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "HP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "HP", nCurValue );

        return true;
    }

    return false;
}

bool NFCPropertyModule::AddMP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "MP" );
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt( self, "MAXMP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeMP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "MP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughMP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "MP");
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::FullSP( const NFGUID& self )
{
    NFINT64 nMAXCSP = m_pKernelModule->GetPropertyInt( self, "MAXSP" );
    if ( nMAXCSP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "SP", nMAXCSP );

        return true;
    }

    return false;
}

bool NFCPropertyModule::AddSP( const NFGUID& self, const NFINT64& nValue )
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "SP" );
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt( self, "MAXSP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, "SP", nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeSP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCSP = m_pKernelModule->GetPropertyInt( self, "SP" );
    if ( ( nCSP > 0 ) && ( nCSP - nValue >= 0 ) )
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt( self, "SP", nCSP );

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughSP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "SP");
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::AddMoney( const NFGUID& self, const NFINT64& nValue )
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "Money" );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeMoney( const NFGUID& self, const NFINT64& nValue )
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, "Money" );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

        return true;
    }    

    return false;
}

bool NFCPropertyModule::EnoughMoney(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "Money");
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::AddDiamond(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "Diamond");
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, "Money", nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeDiamond(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "Diamond");
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, "Diamond", nCurValue);

        return true;
    }    

    return false;
}

bool NFCPropertyModule::EnoughDiamond(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, "Diamond");
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}