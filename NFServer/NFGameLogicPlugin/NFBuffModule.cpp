/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFBuffModule.h"

bool NFBuffModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    //m_pBuffConfigModule = pPluginManager->FindModule<NFIBuffConfigModule>();


    return true;
}


bool NFBuffModule::Shut()
{
    return true;
}

bool NFBuffModule::Execute()
{
    return true;
}

bool NFBuffModule::AfterInit()
{
    return true;
}

bool NFBuffModule::AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent )
{
    
    
    
    
    NFIBuffConfigModule::NFBuffConfig* pBuffConfig = m_pBuffConfigModule->GetBuffConfig( strConfig );
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

bool NFBuffModule::RemoveBuff( const NFGUID& self, const std::string& strConfig )
{
    return true;
}

bool NFBuffModule::RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType )
{
    return true;
}

bool NFBuffModule::SelfFree( const NFGUID& self )
{
    return true;
}

bool NFBuffModule::HasBuff( const NFGUID& self, const std::string& strConfig )
{
    return true;
}

int NFBuffModule::Attack( const NFGUID& self, const NFGUID& other )
{
    return 0;
}

int NFBuffModule::OnDead( const NFGUID& self )
{
    return 0;
}

bool NFBuffModule::ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return true;
}

int NFBuffModule::ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
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

int NFBuffModule::ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
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

int NFBuffModule::ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    //buff group property
    //int nBuffGroup = 0;
    //RUNTIME_BUFF_INFO
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    NF_SHARE_PTR<NFIRecord> pBuffRecord = pObject->GetRecordManager()->GetElement("");
    if ( pBuffRecord )
    {
        NF_SHARE_PTR<NFIRecord> pPropertyGroupRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::CommValue::ThisName());

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

int NFBuffModule::ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFBuffModule::ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFBuffModule::ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFBuffModule::ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
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

int NFBuffModule::ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
{
    return 0;
}

int NFBuffModule::ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
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

int NFBuffModule::ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
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

int NFBuffModule::ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFBuffModule::ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFBuffModule::ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ProcessDeBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }

    return 0;
}

int NFBuffModule::ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent )
{
    return 0;
}

int NFBuffModule::ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
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

int NFBuffModule::ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
{
    return 0;
}

int NFBuffModule::ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        
        ReverseControlPropertyReferAbsoluteValue( self, pBuffConfig );
    }
    return 0;
}

int NFBuffModule::ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig )
{
    return 0;
}

std::string NFBuffModule::GetGroupBuffIndex( const NFGUID& self, int nGroupID )
{
    return "";
}

void NFBuffModule::SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime )
{

}

