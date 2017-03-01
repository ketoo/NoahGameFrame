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
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return  0;
	}

	NFDataList varList;
	if (pHeroRecord->FindString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemSubType());

    return 1;
}

int NFCItemCardConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID )
{
	NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHero)
	{
		return  0;
	}

	NFDataList varList;
	if (pHero->FindString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemSubType());

	NFGUID xHeroID = m_pHeroModule->AddHero(self, strItemID);
	if (xHeroID.IsNull())
	{
		return 0;
	}

	const std::string& strAwardProperty = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::AwardProperty());
	if (!strAwardProperty.empty())
	{
		AwardItemProperty(self, xHeroID, strAwardProperty);
	}

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemID, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}


bool NFCItemCardConsumeProcessModule::AwardItemProperty(const NFGUID& self, const NFGUID& xHeroID, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHero.get())
	{
		return  0;
	}

	NFDataList varList;
	if (pHero->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xHeroID, varList) <= 0)
	{
		return 0;
	}

	if (!m_pElementModule->ExistElement(strID))
	{
		return false;
	}


	const int nVIPLevel = m_pKernelModule->GetPropertyInt(self, NFrame::Player::VIPLevel());
	const int nVIPEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::VIPEXP());
	const int nEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());

	int nAddExp = nVIPLevel * nVIPEXP + nEXP;
	if (nAddExp > 0)
	{
		m_pHeroModule->AddHeroExp(self, xHeroID, nEXP);
	}

	return true;
}
