// -------------------------------------------------------------------------
//    @FileName				:    NFCMapModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCMapModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCMapModule.h"

bool NFCMapModule::Init()
{

    return true;
}

bool NFCMapModule::Shut()
{
    return true;
}

bool NFCMapModule::Execute()
{
    return true;
}

bool NFCMapModule::AfterInit()
{

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_BIG_MAP_GRID, this, &NFCMapModule::ReqBigMapsInfo)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_INFO, this, &NFCMapModule::ReqMapTitleInfo)) { return false; }

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_HOLD_MAP_GRID, this, &NFCMapModule::ReqStation)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_GET_MAP_GRID_AWARD, this, &NFCMapModule::ReqGetMapAward)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_LEAVE_MSG_MAP_GRID, this, &NFCMapModule::ReqLeaveMsgToMap)) { return false; }

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_HUNTING, this, &NFCMapModule::ReqMapHunting)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_KING_WAR, this, &NFCMapModule::ReqMapKingWar)) { return false; }

    return true;
}
	
void NFCMapModule::ReqMapTitleInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqBigMapGridInfo);




}

void NFCMapModule::ReqBigMapsInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqBigMapInfo);
}

void NFCMapModule::ReqStation(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqHoldMapGrid);
}

void NFCMapModule::ReqGetMapAward(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqGetMapAward);
}

void NFCMapModule::ReqLeaveMsgToMap(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqLeaveMapMsg);
}

void NFCMapModule::ReqMapHunting(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMapHunting);
}

void NFCMapModule::ReqMapKingWar(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMapKingWar);
}