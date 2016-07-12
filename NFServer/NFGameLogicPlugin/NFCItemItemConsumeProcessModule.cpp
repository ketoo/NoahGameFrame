// -------------------------------------------------------------------------
//    @FileName      :   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
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


int NFCItemItemConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
{
	return 1;
}

int NFCItemItemConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID)
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

	NFCDataList varItemID;
	const int nBagItemCount = pBagItemList->FindString(NFrame::Player::BagItemList_ConfigID, strItemConfigID, varItemID);
	if (nBagItemCount != 1)
	{
		return 0;
	}

	const int nBagList_ItemCount = varItemID.Int(NFrame::Player::BagItemList_ItemCount);
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
	case NFMsg::EGameItemSubType::EGIT_ITEM_WATER:		// 圣水
	{
		//don't know what to do, what is shengshui?
	}
	break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_DIAMOND:	// 钻石
	case NFMsg::EGameItemSubType::EGIT_ITEM_CURRENCY:	// 金币
	case NFMsg::EGameItemSubType::EGIT_ITEM_EXP:		// EXP
	case NFMsg::EGameItemSubType::EGIT_ITEM_HP:			// 生命药水
	case NFMsg::EGameItemSubType::EGIT_ITEM_MP:			// 魔法药水
	case NFMsg::EGameItemSubType::EGIT_ITEM_SP:			// 体力药水
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
		const NFINT64 nMoney = m_pElementModule->GetPropertyInt(strAwardProperty, NFrame::ConsumeData::Money());

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
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP()) + nHP * nItemCount);
		}
		if (nSP != 0)
		{
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP()) + nSP * nItemCount);
		}
		if (nMP != 0)
		{
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP()) + nMP * nItemCount);
		}
		if (nGold != 0)
		{
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold()) + nGold * nItemCount);
		}
		if (nMoney != 0)
		{
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money()) + nMoney);
		}

		//deduct Bag ItemCount
		const int nRow = varItemID.Int(0);
		pBagItemList->SetInt(nRow, NFrame::Player::BagItemList_ItemCount, nBagList_ItemCount - nItemCount);
	}
	break;
	default:
		break;
	}

	return 1;
}
