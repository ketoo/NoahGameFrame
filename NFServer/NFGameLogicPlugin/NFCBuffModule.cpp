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
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pBuffConfigModule = dynamic_cast<NFIBuffConfigModule*>( pPluginManager->FindModule( "NFCBuffConfigModule" ) );
    

    //  m_pEventProcessModule->AddClassCallBack( "Player", OnObjectClassEvent );
    //  m_pEventProcessModule->AddClassCallBack( "NPC", OnObjectClassEvent );
    mstrRunTimeEffectTable = "RuntimeBuffInfo";
    mstrPropertyTable = "CommPropertyValue";


    return true;
}


bool NFCBuffModule::Shut()
{
    return true;
}

bool NFCBuffModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCBuffModule::Execute( const NFIDENTID& self, float fPassTime )
{
    return true;
}

bool NFCBuffModule::AfterInit()
{
    return true;
}

bool NFCBuffModule::AddBuff( const NFIDENTID& self, const std::string& strConfig, const NFIDENTID& releaserIdent )
{
    //1 死亡判断
    //2 最大MAX判断
    //3 是否BUFF免疫判断
    //4 BUFF是否存在判断
    NFIBuffConfigModule::NFCBuffConfig* pBuffConfig = m_pBuffConfigModule->GetBuffConfig( strConfig );
    if ( pBuffConfig )
    {
        //5 是否有此BUFF判断，有则更新时间
        if ( HasBuff( self, strConfig ) )
        {
            //更新时间
            SetBuffTime( self, strConfig, pBuffConfig->EffectTimeValue, pBuffConfig->EffectTimeInterval );
        }
        else
        {
            //6 同组BUFF判断
            if ( pBuffConfig->BuffGroupIDValue > 0 )
            {
                std::string strOldConfig = GetGroupBuffIndex( self, pBuffConfig->BuffGroupIDValue );
                if ( !strOldConfig.empty() )
                {
                    //同组只能一个BUFF，删掉老的
                    RemoveBuff( self, strOldConfig );
                }
            }

            //不同的类型
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

bool NFCBuffModule::RemoveBuff( const NFIDENTID& self, const std::string& strConfig )
{
    return true;
}

bool NFCBuffModule::RemoveBuff( const NFIDENTID& self, NFIBuffConfigModule::BuffType eType )
{
    return true;
}

bool NFCBuffModule::SelfFree( const NFIDENTID& self )
{
    return true;
}

bool NFCBuffModule::HasBuff( const NFIDENTID& self, const std::string& strConfig )
{
    return true;
}

int NFCBuffModule::Attack( const NFIDENTID& self, const NFIDENTID& other )
{
    return 0;
}

int NFCBuffModule::OnDead( const NFIDENTID& self )
{
    return 0;
}

bool NFCBuffModule::ApplyRoleEffectValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return true;
}

int NFCBuffModule::ProcessBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //属性修改
        ProcessBuffValueProperty( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //控制门
        ProcessBuffControlProperty( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_RATIO == pBuffConfig->EffectValueTypeValue )
    {
        //百分比属性
        ProcessBuffValuePropertyReferRatioValue( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        //绝对值属性
        ProcessBuffValuePropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    //buff group property
    int nBuffGroup = 0;
    //RUNTIME_BUFF_INFO
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    NF_SHARE_PTR<NFIRecord> pBuffRecord = pObject->GetRecordManager()->GetElement( mstrRunTimeEffectTable );
    if ( pBuffRecord )
    {
        NF_SHARE_PTR<NFIRecord> pPropertyGroupRecord = pObject->GetRecordManager()->GetElement( mstrPropertyTable );

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

                //相继设置属性到buff group,增值
                //从属性系统得到属性应该在的col函数
                int nPropertyGroupCol = 0;
                int nPropertyBuffGroupRow = 0;
                TDATA_TYPE eColType = pPropertyGroupRecord->GetColType( nPropertyGroupCol );

                if ( NFIBuffConfigModule::BuffReverseType::ERT_NEED_REVERSE == pBuffConfig->NeedReverseType )
                {
                    //需要还原
                    switch ( eColType )
                    {
                        case TDATA_INT:
                            pPropertyGroupRecord->SetInt( nPropertyBuffGroupRow, nPropertyGroupCol, *pnEffectValue );
                            break;

                        case TDATA_FLOAT:
                            pPropertyGroupRecord->SetFloat( nPropertyBuffGroupRow, nPropertyGroupCol, float( *pnEffectValue ) );
                            break;

                        case TDATA_DOUBLE:
                            pPropertyGroupRecord->SetDouble( nPropertyBuffGroupRow, nPropertyGroupCol, double( *pnEffectValue ) );
                            break;

                        default:
                            break;
                    }
                }
                else if ( NFIBuffConfigModule::BuffReverseType::ERT_NO_REVERSE == pBuffConfig->NeedReverseType )
                {
                    //不需要还原
                    NFIDataList::TData valueEffectValue;
                    switch ( eColType )
                    {
                        case TDATA_INT:
                            valueEffectValue.nType = TDATA_INT;
                            valueEffectValue.variantData =  NFINT64( *pnEffectValue );
                            break;

                        case TDATA_FLOAT:
                            valueEffectValue.nType = TDATA_FLOAT;
                            valueEffectValue.variantData =  float( *pnEffectValue );
                            break;

                        case TDATA_DOUBLE:
                            valueEffectValue.nType = TDATA_DOUBLE;
                            valueEffectValue.variantData = double( *pnEffectValue );
                            break;

                        default:
                            break;
                    }

                    //const NFIDataList& oldValue = pProperty->GetValue();
                    pObject->GetPropertyManager()->SetProperty( strPropertyName, valueEffectValue );
                }

                pnEffectValue = pBuffConfig->Next( strPropertyName );
            }

            //还原与否，都需要保存在runtimebuff表
            NFCDataList valueBuffProperty;
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

int NFCBuffModule::ProcessBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        //绝对值属性
        ProcessBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }
    return 0;
}

int NFCBuffModule::ProcessBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ReverseBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    //属性和控制门
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //属性还原
        ReverseBuffValueProperty( self, pBuffConfig );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //控制门还原
        //ProcessBuffControlProperty(self, pBuffConfig);
    }

    return 0;
}

int NFCBuffModule::ReverseBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //属性修改
        ProcessDeBuffValueProperty( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //DEBUFF控制门原则上不允许
        //ProcessDeBuffControlProperty(self, pBuffConfig, releaserIdent);
    }
    return 0;
}

int NFCBuffModule::ProcessDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_RATIO == pBuffConfig->EffectValueTypeValue )
    {
        //百分比属性
        ProcessDeBuffValuePropertyReferRatioValue( self, pBuffConfig, releaserIdent );
    }
    else if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        //绝对值属性
        ProcessDeBuffValuePropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }

    return 0;
}

int NFCBuffModule::ProcessDeBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ProcessDeBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        //控制门--绝对值
        ProcessDeBuffControlPropertyReferAbsoluteValue( self, pBuffConfig, releaserIdent );
    }

    return 0;
}

int NFCBuffModule::ProcessDeBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent )
{
    return 0;
}

int NFCBuffModule::ReverseDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    //属性和控制门
    if ( NFIBuffConfigModule::BuffSubType::BST_FIX_VALUE_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //属性还原
        ReverseDeBuffValueProperty( self, pBuffConfig );
    }
    else if ( NFIBuffConfigModule::BuffSubType::BST_FIX_CONTROL_PROPERTY == pBuffConfig->BuffSubTypeValue )
    {
        //DEBUFF控制门原则上不允许
        //ProcessBuffControlProperty(self, pBuffConfig);
    }

    return 0;
}

int NFCBuffModule::ReverseDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

int NFCBuffModule::ReverseControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    if ( NFIBuffConfigModule::BuffEffectValueType::EVT_ABSOLUTEVALUE == pBuffConfig->EffectValueTypeValue )
    {
        //控制门还原，绝对值
        ReverseControlPropertyReferAbsoluteValue( self, pBuffConfig );
    }
    return 0;
}

int NFCBuffModule::ReverseControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig )
{
    return 0;
}

std::string NFCBuffModule::GetGroupBuffIndex( const NFIDENTID& self, int nGroupID )
{
    return "";
}

void NFCBuffModule::SetBuffTime( const NFIDENTID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime )
{

}

