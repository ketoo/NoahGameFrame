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

#include "NFItemScrollConsumeProcessModule.h"

bool NFItemScrollConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_SCROLL, this);

    return true;
}

bool NFItemScrollConsumeProcessModule::AfterInit()
{


    return true;
}

bool NFItemScrollConsumeProcessModule::Shut()
{
    return true;
}

bool NFItemScrollConsumeProcessModule::Execute()
{
    return true;
}


int NFItemScrollConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return  1;
	}

	NFDataList varList;
	if (pHeroRecord->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());

    return 100;
}

int NFItemScrollConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector)
{
	NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHero)
	{
		return  1;
	}

	NFDataList varList;
	if (pHero->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) > 0)
	{
		return 2;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());

	switch (nItemSubType)
	{
	case  NFMsg::EGameScrollSubType::EGTST_TOKEN_HERO_CARD:
	{
		const std::string& strExtendHeroCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());

		NFGUID xHeroID = m_pHeroModule->AddHero(self, strExtendHeroCnfID);
		if (xHeroID.IsNull())
		{
			return 3;
		}
	}
	break;
	case  NFMsg::EGameScrollSubType::EGTST_TOKEN_BUILD:
	{
		const std::string& strBuildingCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());
		if (strBuildingCnfID.empty())
		{
			return 4;
		}
		NF_SHARE_PTR<NFIRecord> pBuildRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
		if (nullptr == pBuildRecord)
		{
			return  1;
		}

		const NFVector3 vPos = vector;
		//const NFVector3 vPos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
		const NFGUID xID = m_pKernelModule->CreateGUID();

		E_SCENE_TYPE eSceneType = (E_SCENE_TYPE)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
		if (eSceneType == E_SCENE_TYPE::SCENE_TYPE_NORMAL)
		{
			NF_SHARE_PTR<NFDataList> xDataList = pBuildRecord->GetInitData();
			xDataList->SetString(NFrame::Player::BuildingList::BuildingCnfID, strBuildingCnfID);
			xDataList->SetObject(NFrame::Player::BuildingList::BuildingGUID, xID);
			xDataList->SetVector3(NFrame::Player::BuildingList::Pos, vPos);
			pBuildRecord->AddRow(-1, *xDataList);

			//and we would tell the world server that we created a building in the world, the others should see it once them come into the scene
		}

		NFDataList xDataArg;
		xDataArg.AddString(NFrame::NPC::Position());
		xDataArg.AddVector3(vPos);
		xDataArg.AddString(NFrame::NPC::MasterID());
		xDataArg.AddObject(self);
		xDataArg.AddString(NFrame::NPC::AIOwnerID());
		xDataArg.AddObject(self);
		xDataArg.AddString(NFrame::NPC::NPCType());
		xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

		m_pKernelModule->CreateObject(xID, nSceneID, nGroupID, NFrame::NPC::ThisName(), strBuildingCnfID, xDataArg);


	}
	break;
	default:
		break;

		return 100;
	}
	return 100;
}