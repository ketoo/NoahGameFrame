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


#include "NFPackModule.h"

bool NFPackModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFPackModule::Shut()
{
    return true;
}

bool NFPackModule::Execute()
{
    
    return true;
}

bool NFPackModule::AfterInit()
{

    return true;
}


const NFGUID& NFPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_OBJECT;
	}

	
	bool bExist = m_pElementModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return NULL_OBJECT;
	}

	int nItemType = m_pElementModule->GetPropertyInt32(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP != nItemType )
	{
		return NULL_OBJECT;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagEquipList::ThisName() );
	if (!pRecord)
	{
		return NULL_OBJECT;
	}

	NFGUID ident = m_pKernelModule->CreateGUID();

	NF_SHARE_PTR<NFDataList> var = pRecord->GetInitData();

	var->SetObject(NFrame::Player::BagEquipList::GUID, ident);
	var->SetString(NFrame::Player::BagEquipList::ConfigID, strConfigName.c_str());
	var->SetInt(NFrame::Player::BagEquipList::Date, pPluginManager->GetNowTime());


	int nAddRow = pRecord->AddRow(-1, *var);
	if (nAddRow > 0)
	{
		return pRecord->GetObject(nAddRow, NFrame::Player::BagEquipList::GUID);
	}

	return NULL_OBJECT;
}

bool NFPackModule::CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount )
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
		return false;
	}

	int nItemType = m_pElementModule->GetPropertyInt32(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP == nItemType )
	{
		CreateEquip(self, strConfigName);

		return false;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SCENE_SINGLE_CLONE
		|| eSceneType == NFMsg::ESceneType::SCENE_MULTI_CLONE)
	{
		return CreateItemInTempBag(self, strConfigName, nCount);
	}
	else
	{
		return CreateItemInNormalBag(self, strConfigName, nCount);
	}
	
	return false;
}

bool NFPackModule::DeleteEquip( const NFGUID& self, const NFGUID& id )
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

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagEquipList::ThisName() );
	if (nullptr == pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, varFindResult);
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

bool NFPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
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
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagItemList::ThisName() );
	if (!pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strItemConfigID, varFindResult);
	if (nFindRowCount > 0)
	{
		int nNeedDelCount = nCount;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
			if (nOldCount > nNeedDelCount)
			{
				int nNewCount = nOldCount - nNeedDelCount;
				pRecord->SetInt(nFindRow, NFrame::Player::BagItemList::ItemCount, nNewCount);
				nNeedDelCount = 0;
			}
			else if (nOldCount == nNeedDelCount)
			{
				pRecord->Remove(nFindRow);
				nNeedDelCount = 0;
			}
			else if (nOldCount < nNeedDelCount)
			{
				pRecord->Remove(nFindRow);
				nNeedDelCount -= nOldCount;
			}
		}

		if (nNeedDelCount <= 0)
		{
			return true;
		}
	}

	return false;
}



bool NFPackModule::EnoughItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
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
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagItemList::ThisName() );
    if (!pRecord)
    {
        return false;
    }

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strItemConfigID, varFindResult);
	if (nFindRowCount > 0)
	{
		int nTotalCount = 0;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
			nTotalCount += nOldCount;
		}

		if (nTotalCount > 0)
		{
			return true;
		}
	}

    return false;
}

bool NFPackModule::CreateItemInNormalBag(const NFGUID & self, const std::string & strConfigName, const int nCount)
{
	NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagItemList::ThisName());
	if (nullptr == pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strConfigName, varFindResult);
	if (nFindRowCount <= 0)
	{
		NF_SHARE_PTR<NFDataList> xRowData = pRecord->GetInitData();

		xRowData->SetString(NFrame::Player::BagItemList::ConfigID, strConfigName);
		xRowData->SetInt(NFrame::Player::BagItemList::ItemCount, nCount);
		xRowData->SetInt(NFrame::Player::BagItemList::Date, pPluginManager->GetNowTime());

		int row = pRecord->AddRow(-1, *xRowData);
		if (row < 0)
		{
			m_pLogModule->LogError(self, " cant add item to bag " + strConfigName);
			return false;
		}
	}
	else
	{
		int nFindRow = varFindResult.Int32(0);
		int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
		int nNewCount = nOldCount + nCount;
		pRecord->SetInt(nFindRow, NFrame::Player::BagItemList::ItemCount, nNewCount);
	}

	return true;
}

bool NFPackModule::CreateItemInTempBag(const NFGUID & self, const std::string & strConfigName, const int nCount)
{
	NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::TempItemList::ThisName());
	if (nullptr == pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::TempItemList::ConfigID, strConfigName, varFindResult);
	if (nFindRowCount <= 0)
	{
		NF_SHARE_PTR<NFDataList> xRowData = pRecord->GetInitData();

		xRowData->SetString(NFrame::Player::TempItemList::ConfigID, strConfigName);
		xRowData->SetInt(NFrame::Player::TempItemList::ItemCount, nCount);

		pRecord->AddRow(-1, *xRowData);
	}
	else
	{
		int nFindRow = varFindResult.Int32(0);
		int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::TempItemList::ItemCount);
		int nNewCount = nOldCount + nCount;
		pRecord->SetInt(nFindRow, NFrame::Player::TempItemList::ItemCount, nNewCount);
	}

	return true;
}
