// -------------------------------------------------------------------------
//    @FileName			:    NFCTileModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFCTileModule
//
// -------------------------------------------------------------------------

#include "NFCTileModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTileModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pBigMapRedisModule = pPluginManager->FindModule<NFIBigMapRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pGuildRedisModule = pPluginManager->FindModule<NFIGuildRedisModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	
    return true;
}

bool NFCTileModule::Shut()
{
    return true;
}

bool NFCTileModule::Execute()
{
    return true;
}

bool NFCTileModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTileModule::OnObjectClassEvent);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGEC_REQ_MINING_TITLE, this, &NFCTileModule::ReqMineTile)) { return false; }
    return true;
}

bool NFCTileModule::GetOnlinePlayerTileData(const NFGUID& self, std::string& strData)
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

void NFCTileModule::ReqMineTile(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqMiningTitle);

	NFGUID xViewOppnentID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::ViewOppnent());
	NFGUID xFightOppnentID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::FightOppnent());
	if (!xViewOppnentID.IsNull() || !xFightOppnentID.IsNull())
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

	SaveTileData(nPlayerID);
	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xData, nPlayerID);
}

bool NFCTileModule::AddTile(const NFGUID & self, const int nX, const int nY, const int nOpr)
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

bool NFCTileModule::AddBuilding(const NFGUID & self, const int nX, const int nY, const NFGUID & id, const std::string & strCnfID)
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

bool NFCTileModule::AddNPC(const NFGUID & self, const int nX, const int nY, const NFGUID & id, const std::string & strCnfID)
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

bool NFCTileModule::RemoveTile(const NFGUID & self, const int nX, const int nY)
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

bool NFCTileModule::RemoveBuilding(const NFGUID & self, const int nX, const int nY, const NFGUID & id)
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

bool NFCTileModule::RemoveNPC(const NFGUID & self, const int nX, const int nY, const NFGUID & id)
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

bool NFCTileModule::SaveTileData(const NFGUID & self)
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

	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
	std::string strData;
	if (xData.SerializeToString(&strData))
	{
		return m_pPlayerRedisModule->SavePlayerTile(nSceneID, self, strData);
	}

	return false;
}

bool NFCTileModule::LoadTileData(const NFGUID & self)
{
	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HomeSceneID());
	LoadTileData(self, nSceneID);

	return false;
}

bool NFCTileModule::LoadTileData(const NFGUID & self, const int nSceneID)
{
	mxTileData.RemoveElement(self);

	std::string strData;
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
	return false;
}

bool NFCTileModule::SendTileData(const NFGUID & self)
{
	NF_SHARE_PTR<TileData> xTileData = mxTileData.GetElement(self);
	if (!xTileData)
	{
		return false;
	}

	bool bNeedSend = false;
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
				bNeedSend = true;

				pTile->set_x(xStateData->x);
				pTile->set_y(xStateData->y);
				pTile->set_opr(xStateData->state);
			}
		}


	}

	NF_SHARE_PTR<NFMapEx<int, TileBuilding>> xStateDataBuilding = xTileData->mxTileBuilding.First();
	for (; xStateDataBuilding; xStateDataBuilding = xTileData->mxTileBuilding.Next())
	{
		NF_SHARE_PTR<TileBuilding> xStateData = xStateDataBuilding->First();
		for (; xStateData; xStateData = xStateDataBuilding->Next())
		{
			//pb
			//xStateData
			NFMsg::TileBuilding* pBuilding = xData.add_building();
			if (pBuilding)
			{
				bNeedSend = true;

				pBuilding->set_x(xStateData->x);
				pBuilding->set_y(xStateData->y);
				pBuilding->set_configid(xStateData->configID);
				*pBuilding->mutable_guid() = NFINetModule::NFToPB(xStateData->ID);
			}
		}
	}

	NF_SHARE_PTR<NFMapEx<int, TileNPC>> xStateDataNPC = xTileData->mxTileNPC.First();
	for (; xStateDataNPC; xStateDataNPC = xTileData->mxTileNPC.Next())
	{
		NF_SHARE_PTR<TileNPC> xStateData = xStateDataNPC->First();
		for (; xStateData; xStateData = xStateDataNPC->Next())
		{
			//pb
			//xStateData
			NFMsg::TileNPC* pNpc = xData.add_npc();
			if (pNpc)
			{
				bNeedSend = true;

				pNpc->set_x(xStateData->x);
				pNpc->set_y(xStateData->y);
				pNpc->set_configid(xStateData->configID);
				*pNpc->mutable_guid() = NFINetModule::NFToPB(xStateData->ID);
			}
		}

	}

	if (bNeedSend)
	{
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xData, self);
	}
	return true;
}

int NFCTileModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//cannot save tile here, because player maybe offline in other people scene
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT == eClassEvent)
	{
		//preload at first, then attach
		LoadTileData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH == eClassEvent)
	{
		//load
		SendTileData(self);

		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::BuildingList::ThisName(), this, &NFCTileModule::OnRecordEvent);
	}

	return 0;
}


int NFCTileModule::OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFGUID xBuildingID = m_pKernelModule->GetRecordObject(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingGUID);
		const std::string& strCnfID = m_pKernelModule->GetRecordString(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingCnfID);
		const NFVector3& vPos = m_pKernelModule->GetRecordVector3(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::Pos);
	
		AddBuilding(self, vPos.X(), vPos.Y(), xBuildingID, strCnfID);
	}
		break;
	case RECORD_EVENT_DATA::Del:
	{
		NFGUID xBuildingID = m_pKernelModule->GetRecordObject(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingGUID);
		const std::string& strCnfID = m_pKernelModule->GetRecordString(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::BuildingCnfID);
		const NFVector3& vPos = m_pKernelModule->GetRecordVector3(self, xEventData.strRecordName, xEventData.nRow, NFrame::Player::BuildingList::Pos);

		RemoveBuilding(self, vPos.X(), vPos.Y(), xBuildingID);
	}
		break;
	default:
		break;
	}

	return 0;
}