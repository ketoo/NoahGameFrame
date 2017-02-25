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

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGEC_REQ_MINING_TITLE, this, &NFCTileModule::ReqMineTile)) { return false; }

    return true;
}

void NFCTileModule::ReqMineTile(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMiningTitle);

	int nX = xMsg.x();
	int nY = xMsg.y();
	int nOpr = xMsg.opr();
	if (1 == nOpr)
	{
		AddTile(nPlayerID, nX, nY);
	}
	else if (0 == nOpr)
	{
		RemoveTile(nPlayerID, nX, nY);
	}
	NFMsg::AckMiningTitle xData;
	NFMsg::AckMiningTitle::TileState* pTile = xData.add_tile();
	if (pTile)
	{
		pTile->set_x(nX);
		pTile->set_y(nY);
		pTile->set_opr(nOpr);
	}


	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xData, nPlayerID);
}

bool NFCTileModule::AddTile(const NFGUID & self, const int nX, const int nY)
{
	return false;
}

bool NFCTileModule::RemoveTile(const NFGUID & self, const int nX, const int nY)
{
	return false;
}
