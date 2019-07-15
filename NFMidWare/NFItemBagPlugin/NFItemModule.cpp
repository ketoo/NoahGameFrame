/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
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

#include "NFItemModule.h"

bool NFItemModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	
	return true;
}

bool NFItemModule::Shut()
{
	return true;
}

bool NFItemModule::Execute()
{
	
	return true;
}

bool NFItemModule::AfterInit()
{

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFItemModule::OnClassObjectEvent);
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ITEM_OBJECT, this, &NFItemModule::OnClientUseItem)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_PICK_ITEM, this, &NFItemModule::OnClientPickItem)) { return false; }


	return true;
}

bool NFItemModule::UseItem(const NFGUID & self, const std::string & strItemID, const NFGUID & xTargetID, const NFVector3& vector)
{
	int nCount = 1;
	if (!m_pElementModule->ExistElement(strItemID) || !m_pKernelModule->GetObject(xTargetID))
	{
		return false;
	}

	if (!ConsumeDataItemProperty(self, strItemID))
	{
		return false;
	}

	if (!m_pPackModule->DeleteItem(self, strItemID, nCount))
	{
		return false;
	}

	NFMsg::EItemType eItemType = (NFMsg::EItemType)m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	NF_SHARE_PTR<NFIItemConsumeocessModule> pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(eItemType);
	if (!pConsumeProcessModule)
	{
		return false;
	}

	switch (eItemType)
	{
	case NFMsg::EItemType::EIT_EQUIP:
	case NFMsg::EItemType::EIT_GEM:
	case NFMsg::EItemType::EIT_SCROLL:
	{
		if (pConsumeProcessModule->ConsumeLegal(self, strItemID, NFDataList(), vector) == 0)
		{
			pConsumeProcessModule->ConsumeProcess(self, strItemID, NFDataList(), vector);
		}
	}
	break;
	case NFMsg::EItemType::EIT_SUPPLY:
	{
		NFDataList xTarget;
		xTarget.AddObject(xTargetID);
		xTarget.AddString(strItemID);	//this is Item Config ID
		xTarget.AddInt(nCount);	//this is Item Count to Consume

		if (pConsumeProcessModule->ConsumeLegal(self, strItemID, xTarget, vector) == 0)
		{
			pConsumeProcessModule->ConsumeProcess(self, strItemID, xTarget, vector);
		}
	}
	break;
	break;
	default:
		break;
	}

	return false;
}

int NFItemModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pEventModule->AddEventCallBack(self, NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS, this, &NFItemModule::OnRequireUseItemPosEvent);
	}

	return 0;
}

int NFItemModule::OnRequireUseItemPosEvent(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var)
{
	return 0;
}

bool NFItemModule::CheckConfig()
{
	NF_SHARE_PTR<NFIClass> pLogicCLass = m_pLogicClassModule->GetElement(NFrame::Item::ThisName());
	if (nullptr == pLogicCLass)
	{
		assert(0);
	}

	const std::vector<std::string>& strIdList = pLogicCLass->GetIDList();
	for (int i = 0; i < strIdList.size(); ++i)
	{
		const std::string& strConfigID = strIdList[i];

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strConfigID);
		if (!pPropertyManager)
		{
			assert(0);
		}
		int nType = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::ItemType());

		if (nType < 0)
		{
			assert(0);
		}
		int nSubType = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::ItemSubType());
		if (nSubType < 0)
		{
			assert(0);
		}

		int nLevel = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::Level());
		if (nLevel < 0)
		{
			assert(0);
		}

		int nQuality = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::Quality());
		if (nQuality < 0)
		{
			assert(0);
		}

		//int nCoolDown = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::CoolDownTime());
		//if (nCoolDown <= 0)
		//{
		//	assert(0);
		//}

		int nOverlayCount = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::OverlayCount());
		if (nOverlayCount <= 0)
		{
			assert(0);
		}


		int nBuyPrice = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::BuyPrice());
		int nSalePrice = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::SalePrice());

		//if (nSalePrice <= 0 || nBuyPrice <= 0)
		if (nSalePrice < 0 || nBuyPrice < 0)
		{
			assert(0);
		}
	}

	return true;
}

int NFItemModule::AddItemEffectDataProperty(const NFGUID& self, const NFGUID& xTarget, const std::string& strItemID)
{
	if (strItemID.empty())
	{
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(xTarget);
	if (NULL == pObject)
	{
		//m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strItemID);
	if (!pPropertyManager)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIProperty> pEffectDataProperty = pPropertyManager->GetElement(NFrame::Item::EffectData());
	if (!pEffectDataProperty)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIPropertyManager> pEffectDataPropertyManager = m_pElementModule->GetPropertyManager(pEffectDataProperty->GetString());
	if (!pEffectDataPropertyManager)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIProperty> pProperty = pEffectDataPropertyManager->First();
	while (pProperty)
	{
		if (pProperty->GetInt() != 0)
		{
			m_pPropertyModule->CalculatePropertyValue(xTarget, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, pProperty->GetInt());
		}

		pProperty = pEffectDataPropertyManager->Next();
	}

	return 0;
}

bool NFItemModule::ConsumeDataItemProperty(const NFGUID& self, const std::string& strID)
{
	if (strID.empty())
	{
		return false;
	}

	//const int nVIPEXP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::VIPEXP());
	//const int64_t nEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());
	const int nHP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::HP());
	const int nSP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::SP());
	const int nMP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::MP());
	const int64_t nGold = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::Gold());
	const int nDiamond = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::Diamond());

	if (nGold > 0 && !m_pPropertyModule->EnoughGold(self, nGold))
	{
		return false;
	}

	if (nHP > 0 && !m_pPropertyModule->EnoughHP(self, nHP))
	{
		return false;
	}

	if (nDiamond > 0 && !m_pPropertyModule->EnoughDiamond(self, nDiamond))
	{
		return false;
	}

	if (nMP > 0 && !m_pPropertyModule->EnoughMP(self, nMP))
	{
		return false;
	}

	if (nSP > 0 && !m_pPropertyModule->EnoughSP(self, nSP))
	{
		return false;
	}

	//////

	if (nGold > 0 && !m_pPropertyModule->ConsumeGold(self, nGold))
	{
		return false;
	}

	if (nHP > 0 && !m_pPropertyModule->ConsumeHP(self, nHP))
	{
		return false;
	}

	if (nDiamond > 0 && !m_pPropertyModule->ConsumeDiamond(self, nDiamond))
	{
		return false;
	}

	if (nMP > 0 && !m_pPropertyModule->ConsumeMP(self, nMP))
	{
		return false;
	}

	if (nSP > 0 && !m_pPropertyModule->ConsumeSP(self, nSP))
	{
		return false;
	}

	return true;
}

void NFItemModule::OnClientUseItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckUseItem);

	const NFGUID& self = NFINetModule::PBToNF(xMsg.user());
	const std::string& strItemID = xMsg.item().item_id();
	const NFGUID xTargetID = NFINetModule::PBToNF(xMsg.targetid());
	const NFVector3 vector = NFINetModule::PBToNF(xMsg.position());
	//const int nCount = xMsg.item().item_count();

	UseItem(nPlayerID, strItemID, xTargetID, vector);
}


void NFItemModule::OnClientPickItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqPickDropItem);

    NF_SHARE_PTR<NFIRecord> xDropItemList = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::DropItemList::ThisName());
    if (xDropItemList)
    {
        const NFGUID& xItemID = NFINetModule::PBToNF(xMsg.item_guid());
        const int nRow = xDropItemList->FindObject(NFrame::Player::DropItemList::GUID, xItemID);
		if (nRow >= 0)
		{
			const std::string& strItemID = xDropItemList->GetString(nRow, NFrame::Player::DropItemList::ConfigID);
			const int nCount = xDropItemList->GetInt(nRow, NFrame::Player::DropItemList::ItemCount);

			xDropItemList->Remove(nRow);

			m_pPackModule->CreateItem(nPlayerID, strItemID, nCount);
		}
    }
}

