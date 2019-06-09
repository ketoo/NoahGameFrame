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

#include "NFWorldPropsModule.h"

bool NFWorldPropsModule::Init()
{

	return true;
}

bool NFWorldPropsModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();


	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ADD_BUILDING, this, &NFWorldPropsModule::OnReqAddBuildingProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_BUILDING_LIST, this, &NFWorldPropsModule::OnReqBuildingsProcess)) { return false; }

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_STORE_BUILDING_LIST, this, &NFWorldPropsModule::OnReqStoreBuildingsProcess)) { return false; }

	return true;
}

bool NFWorldPropsModule::Shut()
{
	return true;
}

bool NFWorldPropsModule::Execute()
{
	return true;
}

void NFWorldPropsModule::OnReqStoreBuildingsProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqStoreSceneBuildings);

	std::string data;
	if (xMsg.SerializeToString(&data))
	{
		m_pPlayerRedisModule->SavePlayerTile(xMsg.home_scene_id(), NFINetModule::PBToNF(xMsg.guid()), data);
	}
}

void NFWorldPropsModule::OnReqAddBuildingProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAddSceneBuilding);

	if (!xMsg.is_home_scene())
	{
		//store it by (position.x / cellWidth=100, position.z / cellWidth=100)
		NFGUID posCell(xMsg.pos().x() / 100, xMsg.pos().z() / 100);
		const std::string& strCellIDSKey = m_pCommonRedisModule->GetCellCacheKey(posCell.ToString());
		const std::string& strSceneKey = m_pCommonRedisModule->GetSceneCacheKey(xMsg.scene_id());

		NF_SHARE_PTR<NFIRedisClient> xRedisClient = m_pNoSqlModule->GetDriverBySuit(strSceneKey);
		if (xRedisClient)
		{
			NFGUID id = NFINetModule::PBToNF(xMsg.guid());
			if (!id.IsNull())
			{
				std::string value;
				if (xRedisClient->HGET(strSceneKey, id.ToString(), value))
				{
					//if we have that building in the database that means the player wants to move the building
					//we would delete the id from the old hashmap
					xRedisClient->HDEL(value, id.ToString());
				}

				xRedisClient->HSET(strSceneKey, id.ToString(), strSceneKey + "_" + strCellIDSKey);

				xRedisClient->HSET(strSceneKey + "_" + strCellIDSKey, id.ToString(), xMsg.SerializeAsString());
			}
		}
	}
}

void NFWorldPropsModule::OnReqBuildingsProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqSceneBuildings);

	NFGUID posCell(xMsg.pos().x() / 100, xMsg.pos().z() / 100);
	const std::string& strSceneKey = m_pCommonRedisModule->GetSceneCacheKey(xMsg.scene_id());
	const std::string& strCellIDSKey = m_pCommonRedisModule->GetCellCacheKey(posCell.ToString());
	NF_SHARE_PTR<NFIRedisClient> xRedisClient = m_pNoSqlModule->GetDriverBySuit(strSceneKey);
	if (xRedisClient)
	{
		//scene_id -> cell_id ->cell_id_ref -> cell_building_list
		std::vector<string_pair> strCellValue;
		if (xRedisClient->HGETALL(strSceneKey + "_" + strCellIDSKey, strCellValue))
		{
			NFMsg::AckSceneBuildings ackSceneBuildings;
			for (int i = 0; i < strCellValue.size(); ++i)
			{
				const string_pair& data = strCellValue[i];
				if (!data.second.empty())
				{
					NFMsg::ReqAddSceneBuilding* pAddBuilding = ackSceneBuildings.add_buildings();
					if (pAddBuilding)
					{
						pAddBuilding->ParseFromString(data.second);
					}
				}
			}

			m_pNetModule->SendMsgPB(NFMsg::EGMI_REQ_ACK_BUILDING_LIST, ackSceneBuildings, nSockIndex);
		}
	}
}
