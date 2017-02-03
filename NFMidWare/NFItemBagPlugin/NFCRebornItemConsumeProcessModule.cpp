// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
// -------------------------------------------------------------------------

#include "NFCRebornItemConsumeProcessModule.h"

bool NFCRebornItemConsumeProcessModule::Init()
{
    return true;
}

bool NFCRebornItemConsumeProcessModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    m_pItemConsumeManagerModule->ResgisterConsumeModule( NFMsg::EItemType::EIT_ITEM_REBORN, this );

    return true;
}

bool NFCRebornItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCRebornItemConsumeProcessModule::Execute()
{
    return true;
}

int NFCRebornItemConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
  
    return 0;
}

int NFCRebornItemConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
	return 0;
}
