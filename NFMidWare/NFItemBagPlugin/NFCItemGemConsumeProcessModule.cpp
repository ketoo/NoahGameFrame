// -------------------------------------------------------------------------
//    @FileName			:   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule

// -------------------------------------------------------------------------
#include "NFCItemGemConsumeProcessModule.h"

bool NFCItemGemConsumeProcessModule::Init()
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

bool NFCItemGemConsumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemGemConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemGemConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemGemConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	return 1;
}

int NFCItemGemConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	return 1;
}
