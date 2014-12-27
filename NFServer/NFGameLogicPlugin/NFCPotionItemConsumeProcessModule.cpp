// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   �������ѻ�����,��ϸ�ľ���ĳ��������������Լ��۳�����
// -------------------------------------------------------------------------

#include "NFCPotionItemConsumeProcessModule.h"

bool NFCPotionItemConsumeProcessModule::Init()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pItemConsumeManagerModule = dynamic_cast<NFIItemConsumeManagerModule*>( pPluginManager->FindModule( "NFCItemConsumeManagerModule" ) );
    m_pPackModule = dynamic_cast<NFIPackModule*>( pPluginManager->FindModule( "NFCPackModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLevelModule = dynamic_cast<NFILevelModule*>( pPluginManager->FindModule( "NFCLevelModule" ) );
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>( pPluginManager->FindModule( "NFCPropertyModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pItemConsumeManagerModule );
    assert( NULL != m_pPackModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLevelModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );

    return true;
}

bool NFCPotionItemConsumeProcessModule::AfterInit()
{
    //ҩ������ҩ����ҩ������ҩˮ(�ӹ���BUFF)������ҩˮ(�ӷ���BUFF)
    m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemSubType::EGIT_ITEM_RECOVER_POTION, this );
    //�����˾���ҩˮ
    m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemSubType::EGIT_ITEM_EXP, this );

    return true;
}

bool NFCPotionItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCPotionItemConsumeProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}


int NFCPotionItemConsumeProcessModule::ConsumeLegal( const NFIDENTID& self, int nItemRowID, const NFIValueList& other )
{
    return 1;
}

int NFCPotionItemConsumeProcessModule::ConsumeSelf( const NFIDENTID& self, int nItemRowID )
{
    //�õ�����-1
    int nCount = m_pPackModule->GetGridCount( self, nItemRowID );
    nCount--;

    if ( nCount >= 0 )
    {
        //���ѳɹ�
        if ( 0 == nCount )
        {
            m_pPackModule->DeleteGrid( self, nItemRowID );

        }
        else
        {
            m_pPackModule->SetGridCount( self, nItemRowID, nCount );
        }

        return 1;
    }

    //ʧ��

    return 0;
}

int NFCPotionItemConsumeProcessModule::ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other )
{
    //����Ч��

    NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager( strItemName );
    if ( pPropertyManager )
    {
        NFIProperty* pItemEffectProperty = pPropertyManager->GetElement( "EffectProperty" );
        NFIProperty* pItemEffectValue = pPropertyManager->GetElement( "EffectValue" );
        if ( pItemEffectProperty && pItemEffectValue )
        {
            NFCValueList valueEffectProperty( pItemEffectProperty->QueryString().c_str(), "," );
            NFCValueList valueEffectValue( pItemEffectValue->QueryString().c_str(), "," );
            if ( valueEffectProperty.GetCount() == valueEffectValue.GetCount() )
            {
                for ( int i = 0; i < valueEffectProperty.GetCount(); i++ )
                {
                    //�ȲⶨĿ���Ƿ��д�����(��ʵ�ǵ��������)
                    for ( int j = 0; j < other.GetCount(); j++ )
                    {
                        NFIDENTID identOther = other.ObjectVal( j );
                        if ( !identOther.IsNull() )
                        {
                            NFIObject* pObject = m_pKernelModule->GetObject( identOther );
                            if ( pObject )
                            {
                                std::string strCurProperty = valueEffectProperty.StringVal( i );
                                std::string strMaxProperty = "MAX" + strCurProperty;
                                NFIProperty* pOtherCurProperty = pObject->GetPropertyManager()->GetElement( strCurProperty );
                                NFIProperty* pOtherMaxProperty = pObject->GetPropertyManager()->GetElement( strMaxProperty );
                                if ( pOtherCurProperty && pOtherMaxProperty )
                                {
                                    //ҩ�ֻ���Ǿ���ֵ���ٷֱȲ�Ҫ�ˣ��ٷֱ���BUFFȥ��
                                    //���ң�ֻ�����ֵ�����֣�����ʹ�ã���ˣ�����ֻ���� HP MP CSP 3������
                                    //��Ҫ���ǣ����ܳ������ֵ���⼸�������Ǹ�������������
                                    //�������HP֮��ģ�����ͨ��ҩ��ֱ���޸����ԣ�����ͨ��BUFF���޸ģ�ֻҪ�Ƿֲ����Զ�ͨ��BUFF�޸�
                                    int nAddValue = boost::lexical_cast<int>( valueEffectValue.StringVal( i ) );

                                    if ( "EXP" == strCurProperty )
                                    {
                                        m_pLevelModule->AddExp( self, nAddValue );
                                    }
                                    else if ( "HP" == strCurProperty )
                                    {
                                        m_pPropertyModule->AddHP( self, nAddValue );
                                    }
                                    else if ( "MP" == strCurProperty )
                                    {
                                        m_pPropertyModule->AddMP( self, nAddValue );
                                    }
                                }
                                else
                                {
                                    m_pLogModule->LogProperty(NFILogModule::NLL_ERROR_NORMAL, self, strCurProperty, "", __FUNCTION__, __LINE__);
                                    return 0;
                                }
                            }
                        }
                    }
                }

                return other.GetCount();
            }
        }
    }

    m_pLogModule->LogProperty(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "", __FUNCTION__, __LINE__);


    return 0;
}
