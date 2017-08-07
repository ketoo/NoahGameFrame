// -------------------------------------------------------------------------
//    @FileName			:   NFCItemCardConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCCardItemConsumeProcessModule

// -------------------------------------------------------------------------

#include "NFCItemCardConsumeProcessModule.h"

bool NFCItemCardConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_CARD, this);

    return true;
}

bool NFCItemCardConsumeProcessModule::AfterInit()
{


    return true;
}

bool NFCItemCardConsumeProcessModule::Shut()
{
    return true;
}

bool NFCItemCardConsumeProcessModule::Execute()
{
    return true;
}


int NFCItemCardConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID )
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return  1;
	}

	NFDataList varList;
	if (pHeroRecord->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());

    return 100;
}

int NFCItemCardConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID )
{
	NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHero)
	{
		return  1;
	}

	NFDataList varList;
	if (pHero->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) > 0)
	{
		return 2;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());
	const std::string& strExtendHeroCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());

	NFGUID xHeroID = m_pHeroModule->ActiviteHero(self, strExtendHeroCnfID);
	if (xHeroID.IsNull())
	{
		return 0;
	}

    return 100;
}