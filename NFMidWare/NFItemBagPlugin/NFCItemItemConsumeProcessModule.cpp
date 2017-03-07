// -------------------------------------------------------------------------
//    @FileName			:   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule

// -------------------------------------------------------------------------
#include "NFCItemItemConsumeProcessModule.h"

bool NFCItemItemConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_ITEM, this);

	return true;
}

bool NFCItemItemConsumeProcessModule::AfterInit()
{
	return true;
}

bool NFCItemItemConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemItemConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemItemConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	return 1;
}

int NFCItemItemConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBagItemList = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagItemList());
	if (nullptr == pBagItemList.get())
	{
		return 0;
	}

	const NFGUID xTargetID = targetID.Object(0);
	const std::string strItemConfigID = targetID.String(1);
	const int nItemCount = targetID.Int(2);

	if (xTargetID.IsNull())
	{
		return 0;
	}

	NFDataList varItemID;
	const int nBagItemCount = pBagItemList->FindString(NFrame::Player::BagItemList_ConfigID, strItemConfigID, varItemID);

	if (nBagItemCount != 1)
	{
		return 0;
	}

	const int nRowNum = varItemID.Int(0);
	NFDataList xRowData;
	pBagItemList->QueryRow(nRowNum, xRowData);

	const int nBagList_ItemCount = xRowData.Int(NFrame::Player::BagItemList_ItemCount);

	if (nItemCount > nBagList_ItemCount || nItemCount < 1)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
	const int nSubItemType = m_pElementModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemSubType());

	if (nItemType != NFMsg::EItemType::EIT_ITEM)
	{
		return 0;
	}

	switch (nSubItemType)
	{
	case NFMsg::EGameItemSubType::EGIT_ITEM_WATER:		
	{
		//don't know what to do, what is shengshui?
	}
	break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_DIAMOND:	
	case NFMsg::EGameItemSubType::EGIT_ITEM_CURRENCY:	
	case NFMsg::EGameItemSubType::EGIT_ITEM_EXP:		// EXP
	case NFMsg::EGameItemSubType::EGIT_ITEM_HP:			
	case NFMsg::EGameItemSubType::EGIT_ITEM_MP:			
	case NFMsg::EGameItemSubType::EGIT_ITEM_SP:			
	{
		const std::string strAwardProperty = m_pElementModule->GetPropertyString(strItemConfigID, NFrame::Item::AwardProperty());
		if (strAwardProperty.length() <= 0)
		{
			return 0;
		}
		const NFINT64 nVIPEXP = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::VIPEXP());
		const NFINT64 nEXP = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::EXP());
		const NFINT64 nHP = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::HP());
		const NFINT64 nSP = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::SP());
		const NFINT64 nMP = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::MP());
		const NFINT64 nGold = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::Gold());
		const NFINT64 nDiamond = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::Money());

		if (self == xTargetID)
		{
			if (nVIPEXP != 0)
			{
				m_pKernelModule->SetPropertyInt(self, NFrame::Player::VIPEXP(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::VIPEXP()) + nVIPEXP * nItemCount);
			}
			if (nEXP != 0)
			{
				m_pKernelModule->SetPropertyInt(self, NFrame::Player::EXP(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::EXP()) + nEXP * nItemCount);
			}
			if (nHP != 0)
			{
				if (!m_pPropertyModule->AddHP(self, nHP * nItemCount))
				{
					return 0;
				}
			}
			if (nSP != 0)
			{
				if (!m_pPropertyModule->AddSP(self, nSP * nItemCount))
				{
					return 0;
				}
			}
			if (nMP != 0)
			{
				if (!m_pPropertyModule->AddMP(self, nMP * nItemCount))
				{
					return 0;
				}
			}
			if (nGold != 0)
			{
				if (!m_pPropertyModule->AddGold(self, nGold * nItemCount))
				{
					return 0;
				}
			}
			if (nDiamond != 0)
			{
				if (!m_pPropertyModule->AddDiamond(self, nDiamond * nItemCount))
				{
					return 0;
				}
			}
		}
		else
		{
			NF_SHARE_PTR<NFIRecord> pPlayerHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
			if (nullptr == pPlayerHero.get())
			{
				return 0;
			}

			if (nEXP != 0)
			{
				if (!m_pHeroModule->AddHeroExp(self, xTargetID, nEXP * nItemCount))
				{
					return 0;
				}
			}
			if (nHP != 0)
			{
				if (!m_pHeroPropertyModule->AddHP(self, xTargetID, nHP * nItemCount))
				{
					return 0;
				}
			}
			if (nMP != 0)
			{
				if (!m_pHeroPropertyModule->AddMP(self, xTargetID, nMP * nItemCount))
				{
					return 0;
				}
			}
		}

		//deduct Bag ItemCount
		pBagItemList->SetInt(nRowNum, NFrame::Player::BagItemList_ItemCount, nBagList_ItemCount - nItemCount);
	}
	break;
	default:
		break;
	}

	return 1;
}
