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
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
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
	return 0;
}

int NFCItemItemConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBagItemList = m_pKernelModule->FindRecord(self, NFrame::Player::BagItemList::ThisName());
	if (!pBagItemList)
	{
		return 1;
	}

	const NFGUID xTargetID = targetID.Object(0);
	const std::string strItemConfigID = targetID.String(1);
	const int nItemCount = targetID.Int32(2);

	if (xTargetID.IsNull())
	{
		return 2;
	}

	NFDataList varItemID;
	const int nBagItemCount = pBagItemList->FindString(NFrame::Player::BagItemList::ConfigID, strItemConfigID, varItemID);

	if (nBagItemCount != 1)
	{
		return 3;
	}

	const int nRowNum = varItemID.Int32(0);
	NFDataList xRowData;
	pBagItemList->QueryRow(nRowNum, xRowData);

	const int nBagList_ItemCount = xRowData.Int32(NFrame::Player::BagItemList::ItemCount);

	if (nItemCount > nBagList_ItemCount || nItemCount < 1)
	{
		return 4;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemConfigID, NFrame::Item::ItemType());
	const int nSubItemType = m_pElementModule->GetPropertyInt32(strItemConfigID, NFrame::Item::ItemSubType());

	if (nItemType != NFMsg::EItemType::EIT_ITEM)
	{
		return 5;
	}

	const int nAwardPropertyValue = m_pElementModule->GetPropertyInt(strItemConfigID, NFrame::Item::AwardProperty());
	if (nAwardPropertyValue <= 0)
	{
		return 6;
	}

	//reduce Bag ItemCount
	pBagItemList->SetInt(nRowNum, NFrame::Player::BagItemList::ItemCount, nBagList_ItemCount - nItemCount);

	switch (nSubItemType)
	{
	case NFMsg::EGameItemSubType::EGIT_ITEM_WATER:		
	{
		//don't know what to do, what is shengshui?
	}
	break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_DIAMOND:
		m_pPropertyModule->AddDiamond(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_CURRENCY:
		m_pPropertyModule->AddGold(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_EXP:
		m_pLevelModule->AddExp(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_HP:	
		m_pPropertyModule->AddHP(xTargetID, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_MP:
		m_pPropertyModule->AddMP(xTargetID, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameItemSubType::EGIT_ITEM_SP:
		m_pPropertyModule->AddSP(xTargetID, nAwardPropertyValue * nItemCount);
		break;

	default:
		break;
	}

	return 100;
}
