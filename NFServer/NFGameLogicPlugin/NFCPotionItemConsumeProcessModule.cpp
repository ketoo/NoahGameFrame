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
    

    return true;
}

bool NFCPotionItemConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    m_pItemConsumeManagerModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_ITEM_POSITION, this);
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
