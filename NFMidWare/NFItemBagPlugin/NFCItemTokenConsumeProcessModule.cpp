/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#include "NFCItemTokenConsumeProcessModule.h"

bool NFCItemTokenConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_TOKEN, this);

	return true;
}

bool NFCItemTokenConsumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemTokenConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemTokenConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemTokenConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBuildRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
	if (nullptr == pBuildRecord)
	{
		return  1;
	}

	const std::string& strBuildingCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());
	if (strBuildingCnfID.empty())
	{
		return 2;
	}


	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nHomeSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
	if (nSceneID != nHomeSceneID)
	{
		return 3;
	}

	NFDataList varList;
	if (pBuildRecord->FindString(NFrame::Player::BuildingList::BuildingCnfID, strBuildingCnfID, varList) <= 0)
	{
		return 0;
	}

	return 100;
}

int NFCItemTokenConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBuildRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
	if (nullptr == pBuildRecord)
	{
		return  1;
	}

	//const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	//const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());
	const std::string& strBuildingCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());

	const NFVector3 vPos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nHomeSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
	if (nSceneID != nHomeSceneID)
	{
		return 2;
	}

	const NFGUID xID = m_pKernelModule->CreateGUID();


	NF_SHARE_PTR<NFDataList> xDataList = pBuildRecord->GetInitData();
	xDataList->SetString(NFrame::Player::BuildingList::BuildingCnfID, strBuildingCnfID);
	xDataList->SetObject(NFrame::Player::BuildingList::BuildingGUID, xID);
	xDataList->SetVector3(NFrame::Player::BuildingList::Pos, vPos);
	pBuildRecord->AddRow(-1, *xDataList);


	return 100;
}
