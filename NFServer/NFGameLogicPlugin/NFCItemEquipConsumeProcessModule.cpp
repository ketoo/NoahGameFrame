// -------------------------------------------------------------------------
//    @FileName      :   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------
#include "NFCItemEquipConSumeProcessModule.h"

bool NFCItemEquipConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pEquipModule = pPluginManager->FindModule<NFIEquipModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_EQUIP, this);

	return true;
}

bool NFCItemEquipConsumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemEquipConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemEquipConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemEquipConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
{
	return 1;
}

int NFCItemEquipConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
{
	return 1;
}
