/*
This file is part of:
NoahFrame
https://github.com/ketoo/NoahGameFrame

Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#include "NFScenePropsModule.h"

bool NFScenePropsModule::Init()
{

	return true;
}

bool NFScenePropsModule::AfterInit()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFScenePropsModule::OnPlayerClassEvent);
	
	return true;
}

bool NFScenePropsModule::Shut()
{
	return true;
}

bool NFScenePropsModule::Execute()
{
	return true;
}

int NFScenePropsModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::BuildingList::ThisName(), this, &NFScenePropsModule::OnObjectBuildingRecordEvent);

	}
	return 0;
}

int NFScenePropsModule::OnObjectBuildingRecordEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	if (xEventData.nOpType == RECORD_EVENT_DATA::RecordOptype::Add)
	{
		//send message to world to store this building
		NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
		if (pRecord)
		{
			const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
			const NFGUID& id = pRecord->GetObject(xEventData.nRow, NFrame::Player::BuildingList::BuildingGUID);
			const NFVector3& vector = pRecord->GetVector3(xEventData.nRow, NFrame::Player::BuildingList::Pos);
			const std::string& strCnfID = pRecord->GetString(xEventData.nRow, NFrame::Player::BuildingList::BuildingCnfID);
	
			NFMsg::ReqAddSceneBuilding addBuilding;
			addBuilding.set_config_id(strCnfID);
			addBuilding.set_scene_id(nSceneID);
			*addBuilding.mutable_guid() = NFINetModule::NFToPB(id);
			*addBuilding.mutable_pos() = NFINetModule::NFToPB(vector);
			*addBuilding.mutable_master() = NFINetModule::NFToPB(self);

			m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, self.nData64, NFMsg::EGMI_REQ_ADD_BUILDING, addBuilding);
		}
	}

	return 0;
}
