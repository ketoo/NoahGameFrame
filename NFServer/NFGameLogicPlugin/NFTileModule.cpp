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


#include "NFTileModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFTileModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pBigMapRedisModule = pPluginManager->FindModule<NFIBigMapRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	
    return true;
}

bool NFTileModule::Shut()
{
    return true;
}

bool NFTileModule::Execute()
{
    return true;
}

bool NFTileModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFTileModule::OnObjectClassEvent);

	m_pSceneModule->AddBeforeEnterSceneGroupCallBack(this, &NFTileModule::BeforeEnterSceneGroupEvent);
	m_pSceneModule->AddAfterEnterSceneGroupCallBack(this, &NFTileModule::AfterEnterSceneGroupEvent);
	m_pSceneModule->AddBeforeLeaveSceneGroupCallBack(this, &NFTileModule::BeforeLeaveSceneGroupEvent);
	m_pSceneModule->AddAfterLeaveSceneGroupCallBack(this, &NFTileModule::AfterLeaveSceneGroupEvent);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGEC_REQ_MINING_TITLE, this, &NFTileModule::OnReqMineTileProcess)) { return false; }
    return true;
}

bool NFTileModule::GetOnlinePlayerTileData(const NFGUID& self, std::string& strData)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		return false;
	}

	NFMsg::AckMiningTitle xData;
	NF_SHARE_PTR<NFMapEx<int, TileState>> xStateDataMap = xTileData->mxTileState.First();
	for (; xStateDataMap; xStateDataMap = xTileData->mxTileState.Next())
	{
		NF_SHARE_PTR<TileState> xStateData = xStateDataMap->First();
		for (; xStateData; xStateData = xStateDataMap->Next())
		{
			//pb
			//xStateData
			NFMsg::TileState* pTile = xData.add_tile();
			if (pTile)
			{
				pTile->set_x(xStateData->x);
				pTile->set_y(xStateData->y);
				pTile->set_opr(xStateData->state);
			}
		}
	}

	NF_SHARE_PTR<NFMapEx<int, TileBuilding>> xTileBuildingMap = xTileData->mxTileBuilding.First();
	for (; xTileBuildingMap; xTileBuildingMap = xTileData->mxTileBuilding.Next())
	{
		NF_SHARE_PTR<TileBuilding> xStateData = xTileBuildingMap->First();
		for (; xStateData; xStateData = xTileBuildingMap->Next())
		{
			//pb
			//xStateData
			NFMsg::TileBuilding* pTile = xData.add_building();
			if (pTile)
			{
				pTile->set_x(xStateData->x);
				pTile->set_y(xStateData->y);
				pTile->set_configid(xStateData->configID);
				*pTile->mutable_guid() = NFINetModule::NFToPB(xStateData->ID);
			}


		}
	}

	NF_SHARE_PTR<NFMapEx<int, TileNPC>> xTileNPCMap = xTileData->mxTileNPC.First();
	for (; xTileNPCMap; xTileNPCMap = xTileData->mxTileNPC.Next())
	{
		NF_SHARE_PTR<TileNPC> xStateData = xTileNPCMap->First();
		for (; xStateData; xStateData = xTileNPCMap->Next())
		{
			//pb
			//xStateData
			NFMsg::TileNPC* pTile = xData.add_npc();
			if (pTile)
			{
				pTile->set_x(xStateData->x);
				pTile->set_y(xStateData->y);
				pTile->set_configid(xStateData->configID);
				*pTile->mutable_guid() = NFINetModule::NFToPB(xStateData->ID);
			}


		}
	}

	if (xData.SerializeToString(&strData))
	{
		return true;
	}

	return false;
}

void NFTileModule::OnReqMineTileProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqMiningTitle);

	NFGUID xViewOpponentID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::OpponentID());
	NFGUID xFightOpponentID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::OpponentFighting());
	if (!xViewOpponentID.IsNull() || !xFightOpponentID.IsNull())
	{
		return;
	}

	int nX = xMsg.x();
	int nY = xMsg.y();
	int nOpr = xMsg.opr();
	if (1 == nOpr)//add
	{
		////consume diamond
		AddTile(nPlayerID, nX, nY, nOpr);
	}
	else if (0 == nOpr)//rem
	{
		//get money
		AddTile(nPlayerID, nX, nY, nOpr);
	}

	NFMsg::AckMiningTitle xData;
	NFMsg::TileState* pTile = xData.add_tile();
	if (pTile)
	{
		pTile->set_x(nX);
		pTile->set_y(nY);
		pTile->set_opr(nOpr);
	}

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xData, nPlayerID);
}

bool NFTileModule::AddTile(const NFGUID & self, const int nX, const int nY, const int nOpr)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		xTileData = NF_SHARE_PTR<TileData>(NF_NEW TileData());
		mxTileData.AddElement(self, xTileData);
	}

	NF_SHARE_PTR<NFMapEx<int, TileState>> xStateDataMap = xTileData->mxTileState.GetElement(nX);
	if (!xStateDataMap)
	{
		xStateDataMap = NF_SHARE_PTR<NFMapEx<int, TileState>>(NF_NEW NFMapEx<int, TileState>());
		xTileData->mxTileState.AddElement(nX, xStateDataMap);
	}

	NF_SHARE_PTR<TileState> xTileState  = xStateDataMap->GetElement(nY);
	if (!xTileState)
	{
		xTileState = NF_SHARE_PTR<TileState>(NF_NEW TileState());
		xStateDataMap->AddElement(nY, xTileState);
	}
	else
	{
		if (nOpr == xTileState->state)
		{
			//has be deleted
			return false;
		}
	}
	xTileState->x = nX;
	xTileState->y = nY;
	xTileState->state = nOpr;
	//save
	//SaveTileData(self);

	return true;
}

bool NFTileModule::AddBuilding(const NFGUID & self, const int nX, const int nY, const NFGUID & id, const std::string & strCnfID)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		xTileData = NF_SHARE_PTR<TileData>(NF_NEW TileData());
		mxTileData.AddElement(self, xTileData);
	}

	NF_SHARE_PTR<NFMapEx<int, TileBuilding>> xStateDataMap = xTileData->mxTileBuilding.GetElement(nX);
	if (!xStateDataMap)
	{
		xStateDataMap = NF_SHARE_PTR<NFMapEx<int, TileBuilding>>(NF_NEW NFMapEx<int, TileBuilding>());
		xTileData->mxTileBuilding.AddElement(nX, xStateDataMap);
	}

	NF_SHARE_PTR<TileBuilding> xTileBuilding = xStateDataMap->GetElement(nY);
	if (!xTileBuilding)
	{
		xTileBuilding = NF_SHARE_PTR<TileBuilding>(NF_NEW TileBuilding());
		xStateDataMap->AddElement(nY, xTileBuilding);
	}
	
	xTileBuilding->x = nX;
	xTileBuilding->y = nY;
	xTileBuilding->configID = strCnfID;
	xTileBuilding->ID = id;
	//save
	//SaveTileData(self);

	return true;
}

bool NFTileModule::AddNPC(const NFGUID & self, const int nX, const int nY, const NFGUID & id, const std::string & strCnfID)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		xTileData = NF_SHARE_PTR<TileData>(NF_NEW TileData());
		mxTileData.AddElement(self, xTileData);
	}

	NF_SHARE_PTR<NFMapEx<int, TileNPC>> xStateDataMap = xTileData->mxTileNPC.GetElement(nX);
	if (!xStateDataMap)
	{
		xStateDataMap = NF_SHARE_PTR<NFMapEx<int, TileNPC>>(NF_NEW NFMapEx<int, TileNPC>());
		xTileData->mxTileNPC.AddElement(nX, xStateDataMap);
	}

	NF_SHARE_PTR<TileNPC> xTileNPC = xStateDataMap->GetElement(nY);
	if (!xTileNPC)
	{
		xTileNPC = NF_SHARE_PTR<TileNPC>(NF_NEW TileNPC());
		xStateDataMap->AddElement(nY, xTileNPC);
	}

	xTileNPC->x = nX;
	xTileNPC->y = nY;
	xTileNPC->configID = strCnfID;
	xTileNPC->ID = id;

	return true;
}

bool NFTileModule::RemoveTile(const NFGUID & self, const int nX, const int nY)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<int, TileState>> xStateDataMap = xTileData->mxTileState.GetElement(nX);
	if (!xStateDataMap)
	{
		return false;
	}

	if (xStateDataMap->ExistElement(nY))
	{
		xStateDataMap->RemoveElement(nY);
		//save
		SaveTileData(self);

		return true;
	}

	return false;
}

bool NFTileModule::RemoveBuilding(const NFGUID & self, const int nX, const int nY, const NFGUID & id)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<int, TileBuilding>> xStateDataMap = xTileData->mxTileBuilding.GetElement(nX);
	if (!xStateDataMap)
	{
		return false;
	}

	if (xStateDataMap->ExistElement(nY))
	{
		xStateDataMap->RemoveElement(nY);
		return true;
	}

	return false;
}

bool NFTileModule::RemoveNPC(const NFGUID & self, const int nX, const int nY, const NFGUID & id)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<int, TileNPC>> xStateDataMap = xTileData->mxTileNPC.GetElement(nX);
	if (!xStateDataMap)
	{
		return false;
	}

	if (xStateDataMap->ExistElement(nY))
	{
		xStateDataMap->RemoveElement(nY);
		return true;
	}

	return false;
}

bool NFTileModule::SaveTileData(const NFGUID & self)
{
	std::string strData;
	if (GetOnlinePlayerTileData(self, strData))
	{
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
		//return m_pPlayerRedisModule->SavePlayerTile(nSceneID, self, strData);
	}

	return false;
}

bool NFTileModule::LoadTileData(const NFGUID & self, const int nSceneID)
{
	mxTileData.RemoveElement(self);

	std::string strData;
	/*
	if (m_pPlayerRedisModule->LoadPlayerTile(nSceneID, self, strData))
	{
		NFMsg::AckMiningTitle xData;
		if (xData.ParseFromString(strData))
		{
			for (int i = 0; i <  xData.tile_size(); ++i)
			{
				const NFMsg::TileState& xTile = xData.tile(i);
				AddTile(self, xTile.x(), xTile.y(), xTile.opr());
			}

			for (int i = 0; i < xData.building_size(); ++i)
			{
				const NFMsg::TileBuilding& xTile = xData.building(i);
				AddBuilding(self, xTile.x(), xTile.y(), NFINetModule::PBToNF(xTile.guid()), xTile.configid());
			}

			for (int i = 0; i < xData.npc_size(); ++i)
			{
				const NFMsg::TileNPC& xTile = xData.npc(i);
				AddNPC(self, xTile.x(), xTile.y(), NFINetModule::PBToNF(xTile.guid()), xTile.configid());
			}

			return true;
		}
	}
	*/
	return false;
}

bool NFTileModule::SendTileData(const NFGUID & self)
{
	std::string strData;
	if (GetOnlinePlayerTileData(self, strData))
	{
		m_pGameServerNet_ServerModule->SendMsgToGate(NFMsg::EGEC_ACK_MINING_TITLE, strData, self);
	}

	return true;
}

bool NFTileModule::CreateTileBuilding(const NFGUID & self)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BuildingList::ThisName());
	if (xRecord)
	{
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
		const int nHomeSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

		for (int i = 0; i < xRecord->GetRows(); ++i)
		{
			if (xRecord->IsUsed(i))
			{
				NFGUID xBuildingID = m_pKernelModule->GetRecordObject(self, NFrame::Player::BuildingList::ThisName(), i, NFrame::Player::BuildingList::BuildingGUID);
				const std::string& strCnfID = m_pKernelModule->GetRecordString(self, NFrame::Player::BuildingList::ThisName(), i, NFrame::Player::BuildingList::BuildingCnfID);
				const NFVector3& vPos = m_pKernelModule->GetRecordVector3(self, NFrame::Player::BuildingList::ThisName(), i, NFrame::Player::BuildingList::Pos);
				
				NFDataList xDataArg;
				xDataArg.AddString(NFrame::NPC::Position());
				xDataArg.AddVector3(vPos);
				xDataArg.AddString(NFrame::NPC::MasterID());
				xDataArg.AddObject(self);
				xDataArg.AddString(NFrame::NPC::NPCType());
				xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

				m_pKernelModule->CreateObject(xBuildingID, nHomeSceneID, nGroupID, NFrame::NPC::ThisName(), strCnfID, xDataArg);
			}
		}
	}

	return false;
}

int NFTileModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent)
	{
		SaveTileData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT == eClassEvent)
	{
		//preload at first, then attach
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
		LoadTileData(self, nSceneID);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH == eClassEvent)
	{
		//load
		SendTileData(self);

		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::BuildingList::ThisName(), this, &NFTileModule::OnRecordEvent);
	}

	return 0;
}


int NFTileModule::OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFGUID xBuildingID = m_pKernelModule->GetRecordObject(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingGUID);
		const std::string& strCnfID = m_pKernelModule->GetRecordString(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingCnfID);
		const NFVector3& vPos = m_pKernelModule->GetRecordVector3(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::Pos);
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
		const int nHomeSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

		AddBuilding(self, vPos.X(), vPos.Y(), xBuildingID, strCnfID);

		NFDataList xDataArg;
		xDataArg.AddString(NFrame::NPC::Position());
		xDataArg.AddVector3(vPos);
		xDataArg.AddString(NFrame::NPC::MasterID());
		xDataArg.AddObject(self);
		xDataArg.AddString(NFrame::NPC::NPCType());
		xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

		m_pKernelModule->CreateObject(xBuildingID, nHomeSceneID, nGroupID, NFrame::NPC::ThisName(), strCnfID, xDataArg);
	}
		break;
	case RECORD_EVENT_DATA::Del:
	{
		NFGUID xBuildingID = m_pKernelModule->GetRecordObject(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingGUID);
		//const std::string& strCnfID = m_pKernelModule->GetRecordString(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingCnfID);
		const NFVector3& vPos = m_pKernelModule->GetRecordVector3(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::Pos);

		RemoveBuilding(self, vPos.X(), vPos.Y(), xBuildingID);

		m_pKernelModule->DestroyObject(xBuildingID);
	}
		break;
	default:
		break;
	}

	return 0;
}


int NFTileModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFTileModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//create building if the player back home
	//create building if the player wants attack the others
	/*
	int nPVPType = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::PVPType());
	if (nPVPType == NFMsg::EPVPType::PVP_HOME)
	{
	SendTileData(self);

	//create your building for yourself
	CreateTileBuilding(self);
	}
	*/


	return 0;
}

int NFTileModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFTileModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}
