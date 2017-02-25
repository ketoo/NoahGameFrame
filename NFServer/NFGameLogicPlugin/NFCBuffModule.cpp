// -------------------------------------------------------------------------
//    @FileName         :    NFCBuffModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCBuffModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCBuffModule.h"

bool NFCBuffModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    //m_pBuffConfigModule = pPluginManager->FindModule<NFIBuffConfigModule>();


    mstrRunTimeEffectTable = "RuntimeBuffInfo";

    return true;
}


bool NFCBuffModule::Shut()
{
    return true;
}

bool NFCBuffModule::Execute()
{
    return true;
}

bool NFCBuffModule::Execute( const NFGUID& self, float fPassTime )
{
    return true;
}

bool NFCBuffModule::AfterInit()
{
    return true;
}

bool NFCBuffModule::AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent )
{
    
    
    
    
    NFIBuffConfigModule::NFCBuffConfig* pBuffConfig = m_pBuffConfigModule->GetBuffConfig( strConfig );
    if ( pBuffConfig )
    {
        
        if ( HasBuff( self, strConfig ) )
        {
            
            SetBuffTime( self, strConfig, pBuffConfig->EffectTimeValue, pBuffConfig->EffectTimeInterval );
        }
        else
        {
            
            if ( pBuffConfig->BuffGroupIDValue > 0 )
            {
                std::string strOldConfig = GetGroupBuffIndex( self, pBuffConfig->BuffGroupIDValue );
                if ( !strOldConfig.empty() )
                {
                    
                    RemoveBuff( self, strOldConfig );
                }
            }

            
            if ( NFIBuffConfigModule::BuffType::BT_BUFF == pBuffConfig->BuffTypeValue )
            {
                ProcessBuffValue( self, pBuffConfig, releaserIdent );
            }
            else if ( NFIBuffConfigModule::BuffType::BT_DEBUFF == pBuffConfig->BuffTypeValue )
            {
                ProcessDeBuffValue( self, pBuffConfig, releaserIdent );
            }
        }
    }

    return true;
}

bool NFCBuffModule::RemoveBuff( const NFGUID& self, const std::string& strConfig )
{
    return true;
}

bool NFCBuffModule::RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType )
{
    return true;
}

bool NFCBuffModule::SelfFree( const NFGUID& self )
{
    return true;
}

bool NFCBuffModule::HasBuff( const NFGUID& self, const std::string& strConfig )
{
    return true;
}

int NFCBuffModule::Attack( const NFGUID& self, const NFGUID& other )
{
    return 0;
}

int NFCBuffModule::OnDead( const NFGUID& self )
{
    return 0;
}

bool NFCBuffModule::ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return true;
}

int NFCBuffModule::ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        ProcessBuffValueProperty( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        ProcessBuffControlProperty( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_RATIO == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessBuffValuePropertyReferRatioValue( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessBuffValuePropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    //buff group property
    int nBuffGroup = 0;
    //RUNTIME_BUFF_INFO
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    NF_SHARE_PTR<NFIRecord> pBuffRecord = pObject->GetRecordManager()->GetElement( mstrRunTimeEffectTable );
    if ( pBuffRecord )
    {
        NF_SHARE_PTR<NFIRecord> pPropertyGroupRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_CommPropertyValue());

        std::string strPropertyList;
        std::string strPropertyName;
        int* pnEffectValue = pBuffConfig->First( strPropertyName );
        while ( pnEffectValue )
        {
            NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement( strPropertyName );
            if ( pProperty )
            {
                char szEffectValue[MAX_PATH] = {0};
                printf( szEffectValue, "%d", *pnEffectValue );

                strPropertyList.append( strPropertyName );
                strPropertyList.append( "," );
                strPropertyList.append( szEffectValue );
                strPropertyList.append( ";" );

                
                
                int nPropertyGroupCol = 0;
                int nPropertyBuffGroupRow = 0;
                NFDATA_TYPE eColType = pPropertyGroupRecord->GetColType( nPropertyGroupCol );

                if ( NFIBuffConfigModule::BuffReverseType::ERT_NEED_REVERSE == pBuffConfig->NeedReverseType )
                {
                    
                    switch ( eColType )
                    {
                        case TDATA_INT:
                            pPropertyGroupRecord->SetInt( nPropertyBuffGroupRow, nPropertyGroupCol, *pnEffectValue );
                            break;

                        case TDATA_FLOAT:
                            pPropertyGroupRecord->SetFloat( nPropertyBuffGroupRow, nPropertyGroupCol, double( *pnEffectValue ) );
                            break;

                        default:
                            break;
                    }
                }
                else if ( NFIBuffConfigModule::BuffReverseType::ERT_NO_REVERSE == pBuffConfig->NeedReverseType )
                {
                    
                    NFData valueEffectValue;
                    switch ( eColType )
                    {
                        case TDATA_INT:
							valueEffectValue.SetInt(*pnEffectValue);
                            break;

                        case TDATA_FLOAT:
							valueEffectValue.SetFloat(double(*pnEffectValue));
                            break;

                        default:
                            break;
                    }

                    //const NFDataList& oldValue = pProperty->GetValue();
                    pObject->GetPropertyManager()->SetProperty( strPropertyName, valueEffectValue );
                }

                pnEffectValue = pBuffConfig->Next( strPropertyName );
            }

            
            NFDataList valueBuffProperty;
            valueBuffProperty.AddString( strPropertyName.c_str() );
            valueBuffProperty.AddObject( releaserIdent );
            valueBuffProperty.AddFloat( pBuffConfig->EffectTimeInterval );
            valueBuffProperty.AddInt( pBuffConfig->EffectTimeValue );
            valueBuffProperty.AddString( strPropertyName.c_str() );

            pBuffRecord->AddRow( -1, valueBuffProperty );
        }
    }

    return 0;
}

int NFCBuffModule::ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        ReverseBuffValueProperty( self, pBuffConfig );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        //ProcessBuffControlProperty(self, pBuffConfig);
    }

    return 0;
}

int NFCBuffModule::ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        ProcessDeBuffValueProperty( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        //ProcessDeBuffControlProperty(self, pBuffConfig, releaserIdent);
    }
    return 0;
}

int NFCBuffModule::ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_RATIO == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessDeBuffValuePropertyReferRatioValue( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessDeBuffValuePropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }

    return 0;
}

int NFCBuffModule::ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessDeBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }

    return 0;
}

int NFCBuffModule::ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        ReverseDeBuffValueProperty( self, pBuffConfig );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        
        //ProcessBuffControlProperty(self, pBuffConfig);
    }

    return 0;
}

int NFCBuffModule::ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

int NFCBuffModule::ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ReverseControlPropertyReferAbsoluteValue( self, pBuffConfig );
    }
    return 0;
}

int NFCBuffModule::ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

std::string NFCBuffModule::GetGroupBuffIndex( const NFGUID& self, int nGroupID )
{
    return "";
}

void NFCBuffModule::SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime )
{

}

