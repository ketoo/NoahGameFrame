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

#include "NFEquipModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"


bool NFEquipModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFEquipModule::Shut()
{
    return true;
}

bool NFEquipModule::Execute()
{
    return true;
}

bool NFEquipModule::AfterInit()
{
    return true;
}

bool NFEquipModule::IntensifylevelToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nLevel = GetEquipIntensifyLevel(self, xEquipID);

    const int nCostMoney = m_pCommonConfigModule->GetFieldInt("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetFieldInt("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetFieldString("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughGold(self, nCostMoney))
    {
        return false;
    }
    
    if (!m_pPropertyModule->ConsumeGold(self, nCostMoney))
    {
        return false;
    }
    
    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }
 
    if (Ramdom(nLevel, 100))
    {
        return AddEquipIntensifyLevel(self, xEquipID);
    }

    return false;
}

bool NFEquipModule::HoleToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nHoleCount = GetEquipHoleCount(self, xEquipID);
    const int nCostMoney = m_pCommonConfigModule->GetFieldInt("HoleToEquip", lexical_cast<std::string>(nHoleCount), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetFieldInt("HoleToEquip", lexical_cast<std::string>(nHoleCount), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetFieldString("HoleToEquip", lexical_cast<std::string>(nHoleCount), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }

    if (Ramdom(nHoleCount, 10))
    {
        return AddEquipHoleCount(self, xEquipID);
    }

    return true;
}

bool NFEquipModule::InlaystoneToEquip( const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex )
{
	if (xEquipID.IsNull() || self.IsNull() || strStoneID.empty())
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strStoneID))
	{
		return false;
	}

    if (nHoleIndex < NFrame::Player::BagEquipList::InlayStone1
		|| nHoleIndex > NFrame::Player::BagEquipList::InlayStone10)
    {
        return false;
    }

    const int nHoleCount = GetEquipHoleCount(self, xEquipID);
    if ((nHoleIndex - NFrame::Player::BagEquipList::InlayStone1) <= nHoleCount)
    {
        return false;
    }

    const int nHoleID = nHoleIndex - NFrame::Player::BagEquipList::InlayStone1 + 1;

    const int nCostMoney = m_pCommonConfigModule->GetFieldInt("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetFieldInt("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetFieldString("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }


    if (!m_pPackModule->DeleteItem(self, strStoneID, 1))
    {
        return false;
    }

    return SetEquipInlayStoneID(self, xEquipID, nHoleIndex, strStoneID);
}

bool NFEquipModule::ElementlevelToEquip( const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType )
{

    const int nElementIndex = NFrame::Player::BagEquipList::ElementLevel1_FIRE + eElemetType;
    const int nElementLevel = GetEquipElementLevel(self, xEquipID, nElementIndex);

    const int nCostMoney = m_pCommonConfigModule->GetFieldInt("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetFieldInt("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetFieldString("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeGold(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }

    if (Ramdom(nElementLevel, 100))
    {
        const int nAfterLevel = nElementLevel + 1;
        return AddEquipElementLevel(self, xEquipID, nElementIndex);
    }
    else
    {
        const int nIsDownLevel = m_pCommonConfigModule->GetFieldInt("IntensifylevelToEquip", lexical_cast<std::string>(nElementLevel), "IsDownLevel");
        if (nIsDownLevel > 0)
        {
            int nAfterLevel = nElementLevel - 1;
            if (nAfterLevel < 0 )
            {
                nAfterLevel = 0;
            }

            return AddEquipElementLevel(self, xEquipID, nElementIndex);
        }

        return false;
    }

    return false;
}

bool NFEquipModule::Ramdom( const int nNowLevel , const int nMaxLevel)
{
    if (nNowLevel >= nMaxLevel)
    {
        return false;
    }

    if (nNowLevel < 0)
    {
        return false;
    }

    if (nMaxLevel <= 0)
    {
        return false;
    }

    const NFINT64 nRandomNum = m_pKernelModule->Random(0, nMaxLevel);
    if (nRandomNum > nNowLevel && nRandomNum <= nMaxLevel)
    {
        return true;
    }

    return false;
}

bool NFEquipModule::DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull() || hero.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pBagRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::PlayerHero::ThisName());
	if (!pHeroRecord)
	{
		return false;
	}

	NFDataList xEquipDataList;
	pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xEquipDataList);
	if (xEquipDataList.GetCount() != 1)
	{
		return false;
	}

	NFDataList xHeroDataList;
	pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, hero, xHeroDataList);
	if (xHeroDataList.GetCount() != 1)
	{
		return false;
	}

	const int nEquipRow = xEquipDataList.Int32(0);
	const int nHeroRow = xHeroDataList.Int32(0);
	const std::string& strEquipID = pBagRecord->GetString(nEquipRow, NFrame::Player::BagEquipList::ConfigID);
	const int nEquipPos = m_pElementModule->GetPropertyInt32(strEquipID, NFrame::Equip::ItemSubType());
	/*
    if (nEquipRow < 0
		|| nHeroRow < 0
		|| strEquipID.empty()
		|| nEquipPos < 0
		|| nEquipPos > (NFrame::Player::PlayerHero::Equip6 - NFrame::Player::PlayerHero::Equip1))
    {
		return false;
    }

	//so there have any bind?
	//hero, position
	pHeroRecord->SetObject(nHeroRow, nEquipPos + NFrame::Player::PlayerHero::Equip1, id);
	pBagRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList::WearGUID, hero);
*/
    return false;
}

bool NFEquipModule::TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull() || hero.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pBagRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::PlayerHero::ThisName());
	if (!pHeroRecord)
	{
		return false;
	}

	NFDataList xEquipDataList;
	pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xEquipDataList);
	if (xEquipDataList.GetCount() != 1)
	{
		return false;
	}

	NFDataList xHeroDataList;
	pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, id, xHeroDataList);
	if (xHeroDataList.GetCount() != 1)
	{
		return false;
	}

	const int nEquipRow = xEquipDataList.Int32(0);
	const int nHeroRow = xHeroDataList.Int32(0);
	const std::string& strEquipID = pBagRecord->GetString(nEquipRow, NFrame::Player::BagEquipList::ConfigID);
	const int nEquipPos = m_pElementModule->GetPropertyInt32(strEquipID, NFrame::Equip::ItemSubType());
	/*
	if (nEquipRow < 0
		|| nHeroRow < 0
		|| strEquipID.empty()
		|| nEquipPos < 0
		|| nEquipPos >(NFrame::Player::PlayerHero::Equip6 - NFrame::Player::PlayerHero::Equip1))
	{
		return false;
	}

	//so there have any bind?
	//hero, position
	pHeroRecord->SetObject(nHeroRow, nEquipPos + NFrame::Player::PlayerHero::Equip1, NFGUID());
	pBagRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList::WearGUID, NFGUID());

	*/
	return true;
}

bool NFEquipModule::SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID)
{
	if (id.IsNull() || self.IsNull() || strPropertyID.empty())
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strPropertyID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return false;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	pRecord->SetString(nRow, NFrame::Player::BagEquipList::RandPropertyID, strPropertyID);

	return true;
}

bool NFEquipModule::AddEquipHoleCount(const NFGUID& self, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return false;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	const int nSoltCount = pRecord->GetInt32(nRow, NFrame::Player::BagEquipList::SlotCount);
	pRecord->SetInt(nRow, NFrame::Player::BagEquipList::SlotCount, nSoltCount + 1);

	return true;
}

int NFEquipModule::GetEquipHoleCount(const NFGUID & self, const NFGUID & id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return false;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	return pRecord->GetInt32(nRow, NFrame::Player::BagEquipList::SlotCount);
}

bool NFEquipModule::SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, const int eIndex, const std::string& strStoneID)
{
	if (id.IsNull() || self.IsNull() || strStoneID.empty())
	{
		return false;
	}

	if (eIndex > NFrame::Player::BagEquipList::InlayStone10
		|| eIndex < NFrame::Player::BagEquipList::InlayStone1)
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strStoneID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return false;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	const int nSoltCount = pRecord->GetInt32(nRow, NFrame::Player::BagEquipList::SlotCount);
	if ((eIndex - NFrame::Player::BagEquipList::InlayStone1) <= nSoltCount)
	{
		return false;
	}

	pRecord->SetString(nRow, eIndex, strStoneID);

	return true;
}

bool NFEquipModule::AddEquipIntensifyLevel(const NFGUID& self, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return false;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	const int nLevel = pRecord->GetInt32(nRow, NFrame::Player::BagEquipList::IntensifyLevel);
	pRecord->SetInt(nRow, NFrame::Player::BagEquipList::IntensifyLevel, nLevel + 1);

	return true;
}

int NFEquipModule::GetEquipIntensifyLevel(const NFGUID & self, const NFGUID & id)
{
	if (id.IsNull() || self.IsNull())
	{
		return -1;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return -1;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return -1;
	}

	const int nRow = xDataList.Int32(0);
	return pRecord->GetInt32(nRow, NFrame::Player::BagEquipList::IntensifyLevel);
}

bool NFEquipModule::AddEquipElementLevel(const NFGUID& self, const NFGUID& id, int eIndex)
{
    if (id.IsNull() || self.IsNull())
    {
        return false;
    }

	if (eIndex > NFrame::Player::BagEquipList::ElementLevel5_POISON
		|| eIndex < NFrame::Player::BagEquipList::ElementLevel1_FIRE)
	{
		return false;
	}

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
    if (!pRecord)
    {
        return false;
    }

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int32(0);
	const int nLevel = GetEquipElementLevel(self, id, eIndex);
	pRecord->SetInt(nRow, eIndex, nLevel + 1);

    return true;
}

int NFEquipModule::GetEquipElementLevel(const NFGUID & self, const NFGUID & id, int eIndex)
{
	if (id.IsNull() || self.IsNull())
	{
		return -1;
	}

	if (eIndex > NFrame::Player::BagEquipList::ElementLevel5_POISON
		|| eIndex < NFrame::Player::BagEquipList::ElementLevel1_FIRE)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::BagEquipList::ThisName());
	if (!pRecord)
	{
		return -1;
	}

	NFDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return -1;
	}

	const int nRow = xDataList.Int32(0);
	return pRecord->GetInt32(nRow, eIndex);
}
