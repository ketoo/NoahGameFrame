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
    

    return true;
}

bool NFCCardItemConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    m_pItemConsumeManagerModule->ResgisterConsumeModule( NFMsg::EItemType::EIT_HERO_CARD, this);

    return true;
}

bool NFCCardItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCCardItemConsumeProcessModule::Execute()
{
    return true;
}


int NFCCardItemConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    return 1;
}

int NFCCardItemConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}
