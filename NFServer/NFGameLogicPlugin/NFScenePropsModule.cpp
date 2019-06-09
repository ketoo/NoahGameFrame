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

	if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_REQ_ACK_BUILDING_LIST, this, &NFScenePropsModule::OnAckBuildingsProcess)) { return false; }

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

void NFScenePropsModule::ReqBuildings(const int sceneID, const NFVector3 & pos)
{
	NFMsg::ReqSceneBuildings reqSceneBuildings;
	reqSceneBuildings.set_scene_id(sceneID);
	*reqSceneBuildings.mutable_pos() = NFINetModule::NFToPB(pos);

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, sceneID, NFMsg::EGMI_REQ_ACK_BUILDING_LIST, reqSceneBuildings);
}

int NFScenePropsModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Position(), this, &NFScenePropsModule::OnPlayePositionEvent);

		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::BuildingList::ThisName(), this, &NFScenePropsModule::OnObjectBuildingRecordEvent);

	}
	return 0;
}

int NFScenePropsModule::OnPlayePositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	NFGUID posNewCell(newVar.GetVector3().X() / 100, newVar.GetVector3().Z() / 100);

	if (mCellPullState.find(posNewCell) == mCellPullState.end())
	{
		mCellPullState[posNewCell] = true;

		const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
		ReqBuildings(sceneID, newVar.GetVector3());
	}

	return 0;
}

int NFScenePropsModule::OnObjectBuildingRecordEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	if (xEventData.nOpType == RECORD_EVENT_DATA::RecordOptype::Add
		|| xEventData.nOpType == RECORD_EVENT_DATA::RecordOptype::Update)
	{
		//send message to world to store this building
		NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
		if (pRecord)
		{
			const int nHomeSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HomeSceneID());
			const std::string& strName = m_pKernelModule->GetPropertyString(self, NFrame::Player::Name());

			NFMsg::ReqStoreSceneBuildings sceneProps;
			sceneProps.set_home_scene_id(nHomeSceneID);
			*(sceneProps.mutable_guid()) = NFINetModule::NFToPB(self);

			for (int i = 0; i < pRecord->GetRows(); ++i)
			{
				if (pRecord->IsUsed(i))
				{
					const NFGUID& id = pRecord->GetObject(i, NFrame::Player::BuildingList::BuildingGUID);
					const NFVector3& vector = pRecord->GetVector3(i, NFrame::Player::BuildingList::Pos);
					const std::string& strCnfID = pRecord->GetString(i, NFrame::Player::BuildingList::BuildingCnfID);

					NFMsg::ReqAddSceneBuilding* pAddBuilding = sceneProps.add_buildings();
					pAddBuilding->set_config_id(strCnfID);
					pAddBuilding->set_master_name(strName);
					pAddBuilding->set_scene_id(nHomeSceneID);
					pAddBuilding->set_is_home_scene(1);
					pAddBuilding->set_is_building(1);

					*(pAddBuilding->mutable_guid()) = NFINetModule::NFToPB(id);
					*(pAddBuilding->mutable_pos()) = NFINetModule::NFToPB(vector);
					*(pAddBuilding->mutable_master()) = NFINetModule::NFToPB(self);
				}
			}

			m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nHomeSceneID, NFMsg::EGMI_REQ_STORE_BUILDING_LIST, sceneProps);
		}
	}

	return 0;
}

void NFScenePropsModule::OnAckBuildingsProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckSceneBuildings);

	for (int i = 0; i < xMsg.buildings_size(); ++i)
	{
		const NFMsg::ReqAddSceneBuilding& xSceneBuilding = xMsg.buildings(i);
		
		const NFGUID guid = NFINetModule::PBToNF(xSceneBuilding.guid());
		if (!m_pKernelModule->ExistObject(guid))
		{
			const int sceneID = xSceneBuilding.scene_id();
			const NFVector3& pos = NFINetModule::PBToNF(xSceneBuilding.pos());
			const NFGUID masterID = NFINetModule::PBToNF(xSceneBuilding.master());
			const std::string& configID = xSceneBuilding.config_id();
			const std::string& masterName = xSceneBuilding.master_name();

			NFDataList arg;
			arg.AddString(NFrame::NPC::Position());
			arg.AddVector3(pos);
			arg.AddString(NFrame::NPC::MasterName());
			arg.AddString(masterName);
			arg.AddString(NFrame::NPC::MasterID());
			arg.AddObject(masterID);
			m_pKernelModule->CreateObject(guid, sceneID, 1, NFrame::NPC::ThisName(),configID, arg);
		}
	}
}
