// -------------------------------------------------------------------------
//    @FileName			:   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule

// -------------------------------------------------------------------------
#include "NFCItemTokenConsumeProcessModule.h"

bool NFCItemTokenConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_TOKEN, this);

	return true;
}

bool NFCItemTokenConsumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemTokenConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemTokenConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemTokenConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return  1;
	}

	NFDataList varList;
	if (pHeroRecord->FindString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemSubType());

	return 100;
}

int NFCItemTokenConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBuild = m_pKernelModule->FindRecord(self, NFrame::Player::R_BuildingList());
	if (nullptr == pBuild)
	{
		return  1;
	}



	const int nItemType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemSubType());
	const std::string& strExtendHeroCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());


	return 100;
}
