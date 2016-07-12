// -------------------------------------------------------------------------
//    @FileName      :   NFCItemEquipConSumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConSumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------
#include "NFCItemGemConSumeProcessModule.h"

bool NFCItemGemConSumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_GEM, this);

	return true;
}

bool NFCItemGemConSumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemGemConSumeProcessModule::Shut()
{
	return true;
}

bool NFCItemGemConSumeProcessModule::Execute()
{
	return true;
}


int NFCItemGemConSumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
{
	return 1;
}

int NFCItemGemConSumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
{
	return 1;
}
