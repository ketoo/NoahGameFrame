// -------------------------------------------------------------------------
//    @FileName      :   NFCBriefSkillConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
//    @Desc             :   �������ѻ�����
// -------------------------------------------------------------------------

#include "NFCBriefSkillConsumeProcessModule.h"

bool NFCBriefSkillConsumeProcessModule::Init()
{
    mstrSkillTableName = "SkillTable";

    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pSkillConsumeManagerModule = dynamic_cast<NFISkillConsumeManagerModule*>( pPluginManager->FindModule( "NFCSkillConsumeManagerModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pSkillModule = dynamic_cast<NFISkillModule*>( pPluginManager->FindModule( "NFCSkillModule" ) );
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>( pPluginManager->FindModule( "NFCPropertyModule" ) );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pSkillConsumeManagerModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSkillModule );
    assert( NULL != m_pPropertyModule );

    return true;
}

bool NFCBriefSkillConsumeProcessModule::AfterInit()
{

    //�����༼��
    m_pSkillConsumeManagerModule->ResgisterConsumeModule( EGameSkillType::EGST_JOBSKILL_BRIEF, this );

    return true;
}

bool NFCBriefSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFCBriefSkillConsumeProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}


int NFCBriefSkillConsumeProcessModule::ConsumeLegal( const NFIDENTID& self, const std::string& skillID,  const NFIValueList& other )
{
    //�Ƿ񱻳�Ĭ,CD,�����

    return 0;
}

int NFCBriefSkillConsumeProcessModule::ConsumeSelf( const NFIDENTID& self, const std::string& skillID )
{
//     NFIObject* pObject = m_pKernelModule->GetObject( self );
//     if ( pObject == NULL )
//     {
//         return 1;
//     }
//     NFIRecord* pRecordSkill = pObject->GetRecordManager()->GetElement( mstrSkillTableName );
//     if ( pRecordSkill == NULL )
//     {
//         return 1;
//     }
// 
//     NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager( skillID );
//     if ( pPropertyManager == NULL )
//     {
//         return 1;
//     }
// 
//     NFIProperty* pSkillConsumeProperty = pPropertyManager->GetElement( "ConsumeProperty" );
//     NFIProperty* pSkillConsumeValue = pPropertyManager->GetElement( "ConsumeValue" );
//     NFIProperty* pSkillConsumeType = pPropertyManager->GetElement( "ConsumeType" );
//     if ( pSkillConsumeProperty == NULL || pSkillConsumeValue == NULL || pSkillConsumeType == NULL )
//     {
//         return 1;
//     }
// 
//     int nConsumeType = pSkillConsumeType->QueryInt();
//     NFCValueList valueConsumeProperty( pSkillConsumeProperty->QueryString().c_str(), "," );
//     NFCValueList valueConsumeValue( pSkillConsumeValue->QueryString().c_str(), "," );
// 
//     if ( valueConsumeProperty.GetCount() != valueConsumeValue.GetCount() )
//     {
//         return 1;
//     }
// 
//     for ( int i = 0; i < valueConsumeProperty.GetCount(); i++ )
//     {
//         std::string strCurProperty = valueConsumeProperty.StringVal( i );
//         std::string strMaxProperty = "MAX" + strCurProperty;
//         NFIProperty* pSelfCurProperty = pObject->GetPropertyManager()->GetElement( strCurProperty );
//         NFIProperty* pSelfMaxProperty = pObject->GetPropertyManager()->GetElement( strMaxProperty );
//         if ( pSelfCurProperty == NULL || pSelfMaxProperty == NULL )
//         {
//             continue;
//         }
// 
//         int nConsumeValue = 0;
//         switch ( nConsumeType )
//         {
//             case ConsumeType_Value:
//                 //����ֵ
//                 nConsumeValue = boost::lexical_cast<int>( valueConsumeValue.StringVal( i ) );
//                 break;
//             case ConsumeType_CurrentPercent:
//                 //��ǰ���ٷֱ�
//                 nConsumeValue = boost::lexical_cast<int>( valueConsumeValue.StringVal( i ) ) / 100.0f * pSelfCurProperty->QueryInt();
//                 break;
//             case ConsumeType_MaxPercent:
//                 //������ٷֱ�
//                 nConsumeValue = boost::lexical_cast<int>( valueConsumeValue.StringVal( i ) ) / 100.0f * pSelfMaxProperty->QueryInt();
//                 break;
//             default:
//                 break;
//         }
// 
//         int nRemainValue = pSelfCurProperty->QueryInt() - nConsumeValue;
//         if ( nRemainValue >= 0 )
//         {
//             pSelfCurProperty->SetInt( nRemainValue );
//         }
//     }

    return 0;
}

int NFCBriefSkillConsumeProcessModule::ConsumeProcess( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList )
{
    NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager( strSkillName );
    if ( pPropertyManager == NULL )
    {
        return 1;
    }

//     NFIProperty* pSkillDamageProperty = pPropertyManager->GetElement( "DamageProperty" );
//     NFIProperty* pSkillDamageValue = pPropertyManager->GetElement( "DamageValue" );
//     NFIProperty* pSkillDamageType = pPropertyManager->GetElement( "DamageType" );
//     NFIProperty* pSkillDamageSubType = pPropertyManager->GetElement( "DamageSubType" );
//     if ( pSkillDamageProperty == NULL || pSkillDamageValue == NULL || pSkillDamageType == NULL || pSkillDamageSubType == NULL )
//     {
//         return 1;
//     }
// 
//     int nDamageType = pSkillDamageType->QueryInt();
//     int nDamageSubType = pSkillDamageSubType->QueryInt();
//     NFCValueList valueEffectProperty( pSkillDamageProperty->QueryString().c_str(), "," );
//     NFCValueList valueEffectValue( pSkillDamageValue->QueryString().c_str(), "," );
// 
//     if ( valueEffectProperty.GetCount() != valueEffectValue.GetCount() )
//     {
//         return 1;
//     }

    //for ( int i = 0; i < valueEffectProperty.GetCount(); i++ )
    {
        //�ȲⶨĿ���Ƿ��д�����(��ʵ�ǵ��������)
        for ( int j = 0; j < other.GetCount(); j++ )
        {
            NFIDENTID identOther = other.ObjectVal( j );
            if ( identOther.IsNull() )
            {
                continue;
            }

            NFIObject* pObject = m_pKernelModule->GetObject( identOther );
            if ( pObject == NULL )
            {
                continue;
            }

            //�̵��˺���ֻ����HP,MP֮������������ֵ������
//             std::string strCurProperty = valueEffectProperty.StringVal( i );
//             std::string strMaxProperty = "MAX" + strCurProperty;
//             NFIProperty* pOtherCurProperty = pObject->GetPropertyManager()->GetElement( strCurProperty );
//             NFIProperty* pOtherMaxProperty = pObject->GetPropertyManager()->GetElement( strMaxProperty );
//             if ( pOtherCurProperty == NULL || pOtherMaxProperty == NULL )
//             {
//                 continue;
//             }

            NFIProperty* pLastProperty = pObject->GetPropertyManager()->GetElement( "LastAttacker" );
            if ( pLastProperty )
            {
                pLastProperty->SetObject( self );
            }
        }
    }
    //m_pKernelModule->LogErrorItem(strSkillName.c_str(), "");

    return 0;
}

int NFCBriefSkillConsumeProcessModule::ConsumeProcessEx( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList )
{
    //����Ч��
    //1�˺�����(���)
    //2BUFF(Get,Send)
//     NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager( strSkillName );
//     if ( pPropertyManager == NULL )
//     {
//         return 1;
//     }
// 
//     NFIProperty* pSkillDamageProperty = pPropertyManager->GetElement( "DamageProperty" );
//     NFIProperty* pSkillDamageValue = pPropertyManager->GetElement( "DamageValue" );
//     NFIProperty* pSkillDamageType = pPropertyManager->GetElement( "DamageType" );
//     NFIProperty* pSkillDamageSubType = pPropertyManager->GetElement( "DamageSubType" );
//     if ( pSkillDamageProperty == NULL || pSkillDamageValue == NULL || pSkillDamageType == NULL || pSkillDamageSubType == NULL )
//     {
//         return 1;
//     }
// 
//     int nDamageType = pSkillDamageType->QueryInt();
//     int nDamageSubType = pSkillDamageSubType->QueryInt();
//     NFCValueList valueEffectProperty( pSkillDamageProperty->QueryString().c_str(), "," );
//     NFCValueList valueEffectValue( pSkillDamageValue->QueryString().c_str(), "," );
// 
//     if ( valueEffectProperty.GetCount() != valueEffectValue.GetCount() )
//     {
//         return 1;
//     }
// 
//     for ( int i = 0; i < valueEffectProperty.GetCount(); i++ )
//     {
//         //�ȲⶨĿ���Ƿ��д�����(��ʵ�ǵ��������)
//         for ( int j = 0; j < other.GetCount(); j++ )
//         {
//             NFIDENTID identOther = other.ObjectVal( j );
//             if ( identOther.IsNull() )
//             {
//                 continue;
//             }
// 
//             NFIObject* pObject = m_pKernelModule->GetObject( identOther );
//             if ( pObject == NULL )
//             {
//                 continue;
//             }
// 
//             //�̵��˺���ֻ����HP,MP֮������������ֵ������
//             std::string strCurProperty = valueEffectProperty.StringVal( i );
//             std::string strMaxProperty = "MAX" + strCurProperty;
//             NFIProperty* pOtherCurProperty = pObject->GetPropertyManager()->GetElement( strCurProperty );
//             NFIProperty* pOtherMaxProperty = pObject->GetPropertyManager()->GetElement( strMaxProperty );
//             if ( pOtherCurProperty == NULL || pOtherMaxProperty == NULL )
//             {
//                 continue;
//             }
// 
//             NFIProperty* pLastProperty = pObject->GetPropertyManager()->GetElement( "LastAttacker" );
//             if ( pLastProperty )
//             {
//                 pLastProperty->SetObject( self );
//             }
// 
//             //�ٷֱȾ������ֵ�İٷֱ�(Ĭ��)
//             int nDamageValue = 0;
//             int nCurValue = pOtherCurProperty->QueryInt();//���ܱ�ʬ
//             if ( nCurValue <= 0 )
//             {
//                 continue;
//             }
// 
//             int nMaxValue = pOtherMaxProperty->QueryInt();
// 
//             switch ( nDamageType )
//             {
//             case 0:
//                 //����ֵ
//                 nDamageValue = boost::lexical_cast<int>( valueEffectValue.StringVal( i ) );
//                 break;
//             case 1:
//                 //��ǰ���ٷֱ�
//                 nDamageValue = boost::lexical_cast<int>( valueEffectValue.StringVal( i ) ) / 100.0f * nCurValue;
//                 break;
//             case 2:
//                 //������ٷֱ�
//                 nDamageValue = boost::lexical_cast<int>( valueEffectValue.StringVal( i ) ) / 100.0f * nMaxValue;
//                 break;
//             default:
//                 break;
//             }
// 
//             if ( 0 == nDamageSubType )
//             {
//                 //��(��Ѫ����)--Ŀǰû�������,����������
//                 nCurValue = m_pPropertyModule->ConsumeHP( identOther, nDamageValue );
//             }
//             else
//             {
//                 //��(��Ѫ����)
//                 nCurValue = m_pPropertyModule->AddHP( identOther, nDamageValue );
//             }
// 
//             damageListValue.AddInt( nDamageValue );
//             if ( nCurValue <= 0 )
//             {
//                 damageResultList.AddInt( ( int )( ( rand() % 2 > 0 ) ? EGameSkillResultsType::EART_KILLEDNORMAL : EGameSkillResultsType::EART_KILLEDNORMAL ) );
//             }
//             else
//             {
//                 damageResultList.AddInt( ( int )( ( rand() % 2 > 0 ) ? EGameSkillResultsType::EART_NORMAL : EGameSkillResultsType::EART_THUMP_BACK ) );
//             }
//         }
//     }

    return 0;
}
