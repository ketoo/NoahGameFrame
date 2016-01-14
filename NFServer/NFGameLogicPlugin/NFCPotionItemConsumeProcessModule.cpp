// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#include "NFCPotionItemConsumeProcessModule.h"

bool NFCPotionItemConsumeProcessModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>("NFCLevelModule");
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );

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
    //药剂，红药，蓝药，攻击药水(加攻击BUFF)，防御药水(加防御BUFF)
//     m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemSubType::EGIT_ITEM_RECOVER_POTION, this );
//     //包括了经验药水
//     m_pItemConsumeManagerModule->ResgisterConsumeModule( EGameItemSubType::EGIT_ITEM_EXP, this );

    return true;
}

bool NFCPotionItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCPotionItemConsumeProcessModule::Execute()
{
    return true;
}


int NFCPotionItemConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    return 1;
}

int NFCPotionItemConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
  

    m_pLogModule->LogProperty(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "", __FUNCTION__, __LINE__);


    return 0;
}
