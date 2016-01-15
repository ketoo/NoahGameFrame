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
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>( "NFCPropertyConfigModule" );
    m_pEquipModuele = pPluginManager->FindModule<NFIEquipModule>( "NFCEquipModule" );
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>( "NFCLevelModule" );
    
    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pPropertyConfigModule );
    assert( NULL != m_pEquipModuele );
    assert( NULL != m_pLevelModule );

    m_pEventProcessModule->AddClassCallBack( NFrame::Player::ThisName(), this, &NFCPropertyModule::OnObjectClassEvent );
    return true;
}

bool NFCPropertyModule::Shut()
{
    return true;
}

bool NFCPropertyModule::Execute()
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
        return m_pKernelModule->GetRecordInt( self, NFrame::Player::R_CommPropertyValue(), eGroupType, strPropertyName );
    }

    return m_pKernelModule->GetPropertyInt(self, strPropertyName);
}

int NFCPropertyModule::SetPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject.get())
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord.get())
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


int NFCPropertyModule::AddPropertyValue( const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject.get())
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
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
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
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
    //计算总值
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

    int nAllValue = 0;
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
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
    if ( strClassName == NFrame::Player::ThisName() )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord.get())
            {
                for(int i =0; i< NPG_ALL; i++)
                {
                    pRecord->AddRow(-1);
                }
            }

            m_pKernelModule->AddPropertyCallBack( self, NFrame::Player::Level(), this, &NFCPropertyModule::OnObjectLevelEvent );

            // TODO:一级属性回调
            m_pKernelModule->AddRecordCallBack( self, NFrame::Player::R_CommPropertyValue(), this, &NFCPropertyModule::OnRecordPropertyEvent );


        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt( self, NFrame::Player::OnlineCount() );
            if ( nOnlineCount <= 0 && m_pKernelModule->GetPropertyInt( self, NFrame::Player::SceneID() ) > 0 )
            {
                //第一次出生，设置基础属性
                m_pKernelModule->SetPropertyInt( self, NFrame::Player::Level(), 1 );

                m_pLevelModule->AddLevelUpAward(self, 1);
            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt( self, NFrame::Player::OnlineCount() );
			m_pKernelModule->SetPropertyInt( self, NFrame::Player::OnlineCount(), (nOnlineCount + 1));

        }
    }

    return 0;
}

int NFCPropertyModule::RefreshBaseProperty( const NFGUID& self )
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
    if (!pRecord.get())
    {
        return 1;
    }

    //初始属性+等级属性(职业决定)
    NFJobType eJobType = ( NFJobType )m_pKernelModule->GetPropertyInt( self, NFrame::Player::Job() );
    int nLevel = m_pKernelModule->GetPropertyInt( self, NFrame::Player::Level() );

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
    NFINT64 nMaxHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());
    if (nMaxHP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nMaxHP);
    }

    NFINT64 nMaxMP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());
    if (nMaxMP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nMaxMP);
    }

    return true;
}

bool NFCPropertyModule::AddHP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());

    if ( nCurValue > 0 )
    {
        nCurValue += nValue;
        if ( nCurValue > nMaxValue )
        {
            nCurValue = nMaxValue;
        }

        m_pKernelModule->SetPropertyInt( self, NFrame::Player::HP(), nCurValue );
    }

    return true;
}

bool NFCPropertyModule::EnoughHP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::ConsumeHP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::HP() );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::HP(), nCurValue );

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

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::MP() );
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::MAXMP() );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, NFrame::Player::MP(), nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeMP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::MP() );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::MP(), nCurValue );

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughMP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::FullSP( const NFGUID& self )
{
    NFINT64 nMAXCSP = m_pKernelModule->GetPropertyInt( self, NFrame::Player::MAXSP() );
    if ( nMAXCSP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::SP(), nMAXCSP );

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

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::SP() );
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::MAXSP() );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt( self, NFrame::Player::SP(), nCurValue );

    return true;
}

bool NFCPropertyModule::ConsumeSP( const NFGUID& self, const NFINT64& nValue )
{
    NFINT64 nCSP = m_pKernelModule->GetPropertyInt( self, NFrame::Player::SP() );
    if ( ( nCSP > 0 ) && ( nCSP - nValue >= 0 ) )
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::SP(), nCSP );

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughSP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
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

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::Gold() );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, NFrame::Player::Gold(), nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeMoney( const NFGUID& self, const NFINT64& nValue )
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt( self, NFrame::Player::Gold() );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::Gold(), nCurValue);

        return true;
    }    

    return false;
}

bool NFCPropertyModule::EnoughMoney(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
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

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeDiamond(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

        return true;
    }    

    return false;
}

bool NFCPropertyModule::EnoughDiamond(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}