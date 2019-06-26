/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: yu.tang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFItemSupplyConsumeProcessModule.h"

bool NFItemSupplyConsumeProcessModule::Init()
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

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_SUPPLY, this);

	return true;
}

bool NFItemSupplyConsumeProcessModule::AfterInit()
{
	return true;
}

bool NFItemSupplyConsumeProcessModule::Shut()
{
	return true;
}

bool NFItemSupplyConsumeProcessModule::Execute()
{
	return true;
}


int NFItemSupplyConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector)
{
	return 0;
}

int NFItemSupplyConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector)
{

	return ConsumeNormalProcess(self, strItemID, targetID, vector);
}

int NFItemSupplyConsumeProcessModule::ConsumeNormalProcess(const NFGUID & self, const std::string & strItemID, const NFDataList & targetID, const NFVector3& vector)
{
	NF_SHARE_PTR<NFIRecord> pBagItemList = m_pKernelModule->FindRecord(self, NFrame::Player::BagItemList::ThisName());
	if (!pBagItemList)
	{
		return 1;
	}

	const NFGUID xTargetID = targetID.Object(0);

	if (xTargetID.IsNull())
	{
		return 2;
	}

	NFDataList varItemID;
	pBagItemList->FindString(NFrame::Player::BagItemList::ConfigID, strItemID, varItemID);
	if (varItemID.GetCount() <= 0)
	{
		return 3;
	}

	const int nRowNum = varItemID.Int32(0);
	if (nRowNum < 0)
	{
		return 4;
	}

	const int nItemCount = pBagItemList->GetInt(nRowNum, NFrame::Player::BagItemList::ItemCount);
	if (nItemCount <= 0)
	{
		pBagItemList->Remove(nRowNum);
		return 5;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nSubItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());

	if (nItemType != NFMsg::EItemType::EIT_SUPPLY)
	{
		return 5;
	}

	const int nAwardPropertyValue = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::AwardProperty());
	if (nAwardPropertyValue <= 0)
	{
		return 6;
	}

	//reduce Bag ItemCount
	if (nItemCount > 1)
	{
		pBagItemList->SetInt(nRowNum, NFrame::Player::BagItemList::ItemCount, nItemCount - 1);
	}
	else
	{
		pBagItemList->Remove(nRowNum);
	}

	switch (nSubItemType)
	{
	case NFMsg::EGameSupplySubType::EGIT_ITEM_WATER:
	{
		//don't know what to do, what is shengshui?
	}
	break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_DIAMOND:
		m_pPropertyModule->AddDiamond(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_CURRENCY:
		m_pPropertyModule->AddGold(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_EXP:
		m_pLevelModule->AddExp(self, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_HP:
		m_pPropertyModule->AddHP(xTargetID, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_MP:
		m_pPropertyModule->AddMP(xTargetID, nAwardPropertyValue * nItemCount);
		break;
	case NFMsg::EGameSupplySubType::EGIT_ITEM_SP:
		m_pPropertyModule->AddSP(xTargetID, nAwardPropertyValue * nItemCount);
		break;

	default:
		break;
	}

	return 100;
}
