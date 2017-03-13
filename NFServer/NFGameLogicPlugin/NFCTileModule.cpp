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

	if (xData.SerializeToString(&strData))
	{
		return true;
	}

	return false;
}

void NFCTileModule::ReqMineTile(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMiningTitle);

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

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HomeSceneID());
	std::string strData;
	if (xData.SerializeToString(&strData))
	{
		return m_pPlayerRedisModule->SavePlayerTile(nSceneID, self, strData);
	}

	return false;
}

bool NFCTileModule::LoadTileData(const NFGUID & self)
{
	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HomeSceneID());
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
			int nCount = xData.tile_size();
			for (int i = 0; i < nCount; ++i)
			{
				const NFMsg::TileState& xTile = xData.tile(i);
				AddTile(self, xTile.x(), xTile.y(), xTile.opr());
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

	if (bNeedSend)
	{
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xData, self);
	}
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
	}

	return 0;
}
