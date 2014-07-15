// -------------------------------------------------------------------------
//    @FileName      :   NFCCardItemConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCCardItemConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#include "NFCCardItemConsumeProcessModule.h"

bool NFCCardItemConsumeProcessModule::Init()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pItemConsumeManagerModule = dynamic_cast<NFIItemConsumeManagerModule*>( pPluginManager->FindModule( "NFCItemConsumeManagerModule" ) );
    m_pPackModule = dynamic_cast<NFIPackModule*>( pPluginManager->FindModule( "NFCPackModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pItemConsumeManagerModule );
    assert( NULL != m_pPackModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogModule );

    return true;
}

bool NFCCardItemConsumeProcessModule::AfterInit()
{
    //学习技能卡暂时废掉
    //m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemType::EGIT_ITEM_SKILL_CARD, this);
    m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemSubType::EGIT_ITEM_PACK, this );

    return true;
}

bool NFCCardItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCCardItemConsumeProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}


int NFCCardItemConsumeProcessModule::ConsumeLegal( const NFIDENTID& self, int nItemRowID, const NFIValueList& other )
{
    return 1;
}

int NFCCardItemConsumeProcessModule::ConsumeSelf( const NFIDENTID& self, int nItemRowID )
{
    //得到数量-1
    int nCount = m_pPackModule->GetGridCount( self, nItemRowID );
    nCount--;

    if ( nCount >= 0 )
    {
        //消费成功
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

    //失败

    return 0;
}

int NFCCardItemConsumeProcessModule::ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other )
{
    //附加效果

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
                    //先测定目标是否有此属性(其实是担心配错了)
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
                                    //技能书之类的，学习新东西(并不是装备类的那个技能书，那个是用来强化的)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}
