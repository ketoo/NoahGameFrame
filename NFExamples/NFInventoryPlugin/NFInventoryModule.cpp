/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#include "NFInventoryModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"

bool NFInventoryModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFInventoryModule::Shut()
{
    return true;
}

bool NFInventoryModule::Execute()
{
    
    return true;
}

bool NFInventoryModule::AfterInit()
{

    return true;
}


NFGUID NFInventoryModule::CreateEquip(const NFGUID& self, const std::string& strConfigName, const int nCount)
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_OBJECT;
	}

	
	bool bExist = m_pElementModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		m_pLogModule->LogError(self, "has no this element:" + strConfigName);
		return NULL_OBJECT;
	}

	int nItemType = m_pElementModule->GetPropertyInt32(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP != nItemType )
	{
		m_pLogModule->LogError(self, strConfigName + " has no this item type:" + std::to_string(nItemType));
		return NULL_OBJECT;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::InventoryEquipment::ThisName() );
	if (!pRecord)
	{
		return NULL_OBJECT;
	}

	NFGUID ident = m_pKernelModule->CreateGUID();

	NF_SHARE_PTR<NFDataList> var = pRecord->GetInitData();

	var->SetObject(NFrame::Player::InventoryEquipment::GUID, ident);
	var->SetString(NFrame::Player::InventoryEquipment::ConfigID, strConfigName.c_str());
	var->SetInt(NFrame::Player::InventoryEquipment::Date, pPluginManager->GetNowTime());

	if (nCount > 1)
	{
		for (int i = 0; i < nCount; ++i)
		{
			var->SetObject(NFrame::Player::InventoryEquipment::GUID, m_pKernelModule->CreateGUID());
			pRecord->AddRow(-1, *var);
		}

		return NULL_OBJECT;
	}
	else if (nCount == 1)
	{
		int nAddRow = pRecord->AddRow(-1, *var);
		if (nAddRow >= 0)
		{
			return pRecord->GetObject(nAddRow, NFrame::Player::InventoryEquipment::GUID);
		}
	}

	return NULL_OBJECT;
}

bool NFInventoryModule::CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount)
{
	if (nCount <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	
	bool bExist = m_pElementModule->ExistElement(NFrame::Item::ThisName(), strConfigName );
	if ( !bExist )
	{
		m_pLogModule->LogError(self, "has no this element:" + strConfigName);
		return false;
	}

	int nItemType = m_pElementModule->GetPropertyInt32(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP == nItemType )
	{
		CreateEquip(self, strConfigName, nCount);

		return false;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());

	return CreateItemInNormalBag(self, strConfigName, nCount);
}

bool NFInventoryModule::DeleteEquip(const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if (nullptr == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::InventoryEquipment::ThisName() );
	if (nullptr == pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindObject(NFrame::Player::InventoryEquipment::GUID, id, varFindResult);
	if (nFindRowCount > 0)
	{
		//int nTotalCount = 0;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			pRecord->Remove(nFindRow);
		}
	}

	return true;
}

bool NFInventoryModule::DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
	if(nCount <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}


	if (!m_pElementModule->ExistElement(NFrame::Item::ThisName(), strItemConfigID))
	{
		m_pLogModule->LogError(self, "has no this element:" + strItemConfigID);
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::Inventory::ThisName() );
	if (!pRecord)
	{
		return false;
	}

	int nFindRow = pRecord->FindString(NFrame::Player::Inventory::ConfigID, strItemConfigID);
	if (nFindRow >= 0)
	{
		int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::Inventory::ItemCount);
		if (nOldCount > nCount)
		{
			int nNewCount = nOldCount - nCount;
			pRecord->SetInt(nFindRow, NFrame::Player::Inventory::ItemCount, nNewCount);

			m_pLogModule->LogInfo(self, " DeleteItem:" + strItemConfigID + ", from " + std::to_string(nOldCount) + " to " + std::to_string(nNewCount));
			return true;
		}
		else if (nOldCount == nCount)
		{
			pRecord->Remove(nFindRow);
			m_pLogModule->LogInfo(self, " DeleteItem:" + strItemConfigID + ", from " + std::to_string(nOldCount) + " to 0");
			return true;
		}
	}

	return false;
}



bool NFInventoryModule::EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
    if(nCount <= 0)
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    
    bool bExist = m_pElementModule->ExistElement(NFrame::Item::ThisName(), strItemConfigID );
    if ( !bExist )
    {
		m_pLogModule->LogError(self, "has no this element:" + strItemConfigID);
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::Inventory::ThisName() );
    if (!pRecord)
    {
        return false;
    }

	int row = pRecord->FindString(NFrame::Player::Inventory::ConfigID, strItemConfigID);
	if (row >= 0)
	{
		int count = pRecord->GetInt32(row, NFrame::Player::Inventory::ItemCount);

		if (count >= nCount)
		{
			return true;
		}
	}

    return false;
}

bool NFInventoryModule::CreateItemInNormalBag(const NFGUID & self, const std::string & strConfigName, const int nCount)
{
	NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::Inventory::ThisName());
	if (nullptr == pRecord)
	{
		return false;
	}

	int row = pRecord->FindString(NFrame::Player::Inventory::ConfigID, strConfigName);
	if (row < 0)
	{
		NF_SHARE_PTR<NFDataList> xRowData = pRecord->GetInitData();

		xRowData->SetString(NFrame::Player::Inventory::ConfigID, strConfigName);
		xRowData->SetInt(NFrame::Player::Inventory::ItemCount, nCount);

		int row = pRecord->AddRow(-1, *xRowData);
		if (row < 0)
		{
			m_pLogModule->LogError(self, " cant add item to bag " + strConfigName);
			return false;
		}
	}
	else
	{
		int count = pRecord->GetInt32(row, NFrame::Player::Inventory::ItemCount) + nCount;
		pRecord->SetInt(row, NFrame::Player::Inventory::ItemCount, count);
	}


	m_pLogModule->LogInfo(self, "add item to bag:" + strConfigName + ", count:" + std::to_string(nCount));

	return true;
}

int NFInventoryModule::ItemCount(const NFGUID &self, const std::string &strItemConfigID)
{

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}


	if (!m_pElementModule->ExistElement(NFrame::Item::ThisName(), strItemConfigID))
	{
		m_pLogModule->LogError(self, "has no this element:" + strItemConfigID);
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::Inventory::ThisName() );
	if (!pRecord)
	{
		return 0;
	}

	int nFindRow = pRecord->FindString(NFrame::Player::Inventory::ConfigID, strItemConfigID);
	if (nFindRow >= 0)
	{
		return pRecord->GetInt32(nFindRow, NFrame::Player::Inventory::ItemCount);
	}

	return 0;
}
