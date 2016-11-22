// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPMatchModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPMatchModule
//
// -------------------------------------------------------------------------

#include "NFCPVPMatchModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"

bool NFCPVPMatchModule::Init()
{

	return true;
}


bool NFCPVPMatchModule::Shut()
{
	return true;
}

bool NFCPVPMatchModule::Execute()
{
	
	NFINT64 nNowTime = pPluginManager->GetNowTime();

	if (nNowTime - mnLastCheckTime < 60)
	{
		return true;
	}
	mnLastCheckTime = nNowTime;

	ProecessWaitRoom();
	ProecessRoomBeginFight();
	ProcessSingePlayerRoom();

	return true;
}

bool NFCPVPMatchModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPVPMatchRedisModule = pPluginManager->FindModule<NFIPVPMatchRedisModule>();
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	m_pTeamModule = pPluginManager->FindModule<NFITeamModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();

	if (!m_pWorldNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_PVPAPPLYMACTCH, this, &NFCPVPMatchModule::OnReqPVPApplyMatchProcess)) { return false; }
	if (!m_pWorldNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_CREATEPVPECTYPE, this, &NFCPVPMatchModule::OnAckCreatePVPEctypeProcess)) { return false; }

	return true;
}

bool NFCPVPMatchModule::ApplyPVP(const NFGUID& self, const int nPVPMode, const int nScore)
{
	int nGrade = GetGradeByScore(nScore);
	return m_pPVPMatchRedisModule->PushSinglePlayer(self, nPVPMode, nGrade);
}

bool NFCPVPMatchModule::TeamApplyPVP(const NFGUID& xTeam, const NFIDataList& varMemberList, int nPVPMode, const int nScore)
{
	int nMemberCount = varMemberList.GetCount();
	std::vector<NFGUID> xPlayerList;
	for (int i = 0; i < varMemberList.GetCount(); i++)
	{
		const NFGUID& xPlayerID = varMemberList.Object(i);
		if (xPlayerID.IsNull())
		{
			return false;
		}

		xPlayerList.push_back(xPlayerID);
	}

	int nGrade = GetGradeByScore(nScore);

	int nPVPModeMaxMember = GetMemberCount(nPVPMode);
	if (nPVPModeMaxMember < 0)
	{
		return false;
	}

	if (nMemberCount > nPVPModeMaxMember)
	{
		return false;
	}

	NFGUID xFindRoomID;
	int bRed = EPVPREDORBLUE_RED;
	std::vector<NFGUID> xRoomIDList;
	if (m_pPVPMatchRedisModule->GetStatusRoomID(nPVPMode, nGrade, EPVPROOMSTATUS_WAIT, xRoomIDList))
	{
		for (int i = 0; i < xRoomIDList.size(); i++)
		{
			const NFGUID& xRoomID = xRoomIDList[i];
			NFMsg::PVPRoomInfo xRoomInfo;
			if (m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
			{
				if ((xRoomInfo.xredplayer_size() + nMemberCount) < xRoomInfo.maxpalyer())
				{
					xFindRoomID = xRoomID;
					bRed = EPVPREDORBLUE_RED;
					break;
				}
				else if ((xRoomInfo.xblueplayer_size() + nMemberCount) < xRoomInfo.maxpalyer())
				{
					xFindRoomID = xRoomID;
					bRed = EPVPREDORBLUE_BULE;
					break;
				}
			}
		}
	}

	if (xFindRoomID.IsNull())
	{
		xFindRoomID = CreateRoom(NFGUID(), nPVPMode, nGrade);
		if (xFindRoomID.IsNull())
		{
			return false;
		}

		m_pPVPMatchRedisModule->SetStatusRoomID(nPVPMode, nGrade, EPVPROOMSTATUS_WAIT, xFindRoomID);
	}

	return PlayerListEnterRoom(xPlayerList, bRed, xFindRoomID);
}

int NFCPVPMatchModule::GetGradeByScore(const int nScore)
{
	return 1;
}

int NFCPVPMatchModule::GetMemberCount(const int nPVPMode)
{
	switch (nPVPMode)
	{
	case EPVPMODE_ONE:
		return 1;
		break;
	case EPVPMODE_TWO:
		return 2;
		break;
	case EPVPMODE_THREE:
		return 3;
		break;
	default:
		return -1;
		break;
	}

	return -1;
}

NFGUID NFCPVPMatchModule::CreateRoom(const NFGUID& self, const int nPVPMode, const int nGrade)
{
	NFGUID xRoomID = m_pKernelModule->CreateGUID();
	if (xRoomID.IsNull())
	{
		return NFGUID();
	}

	NFMsg::PVPRoomInfo xRoomInfo;

	xRoomInfo.set_ncellstatus(0);
	*xRoomInfo.mutable_roomid() = NFINetModule::NFToPB(xRoomID);
	xRoomInfo.set_npvpmode(nPVPMode);
	xRoomInfo.set_npvpgrade(nGrade);

	int nPVPModeMaxMember = GetMemberCount(nPVPMode);
	if (nPVPModeMaxMember < 0)
	{
		return NFGUID();
	}

	xRoomInfo.set_maxpalyer(nPVPModeMaxMember);
	if (!m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
	{
		return NFGUID();
	}

	return xRoomID;
}

bool NFCPVPMatchModule::DestroyRoom(const NFGUID& self, const NFGUID& xRoomID)
{
	return m_pPVPMatchRedisModule->DeletePlayerRoomID(self, xRoomID);
}

bool NFCPVPMatchModule::PlayerEnterRoom(const NFGUID& self, const int nRed, const NFGUID& xRoomID)
{
	if (!xRoomID.IsNull())
	{
		return false;
	}

	NFMsg::PVPRoomInfo xRoomInfo;
	if (!m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	if (nRed == EPVPREDORBLUE_RED)
	{
		if ((xRoomInfo.xredplayer_size() + 1) > xRoomInfo.maxpalyer())
		{
			return false;
		}

		*xRoomInfo.add_xredplayer() = NFINetModule::NFToPB(self);
	}
	else
	{
		if ((xRoomInfo.xblueplayer_size() + 1) < xRoomInfo.maxpalyer())
		{
			return false;
		}

		*xRoomInfo.add_xblueplayer() = NFINetModule::NFToPB(self);
	}

	if (!m_pPVPMatchRedisModule->SetPlayerRoomID(self, xRoomID))
	{
		return false;
	}

	UpdateRoomStatus(xRoomInfo);

	if (!m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	return true;
}

bool NFCPVPMatchModule::PlayerListEnterRoom(const std::vector<NFGUID>& xPlayerList, const int nRedOrBlue, const NFGUID& xRoomID)
{
	if (!xRoomID.IsNull())
	{
		return false;
	}

	NFMsg::PVPRoomInfo xRoomInfo;
	if (!m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	if (nRedOrBlue == EPVPREDORBLUE_RED)
	{
		if ((xRoomInfo.xredplayer_size() + xPlayerList.size()) > xRoomInfo.maxpalyer())
		{
			return false;
		}

		for (int i = 0; i < xPlayerList.size(); i++)
		{
			const NFGUID& xPlayerID = xPlayerList[i];
			if (xPlayerID.IsNull())
			{
				continue;
			}

			NFGUID xOldRoomID;
			if (!m_pPVPMatchRedisModule->GetPlayerRoomID(xPlayerID, xOldRoomID))
			{
				continue;
			}

			if (!xOldRoomID.IsNull())
			{
				continue;
			}

			if (!m_pPVPMatchRedisModule->SetPlayerRoomID(xPlayerID, xRoomID))
			{
				continue;
			}

			*xRoomInfo.add_xredplayer() = NFINetModule::NFToPB(xPlayerID);
		}
	}
	else
	{
		if ((xRoomInfo.xblueplayer_size() + xPlayerList.size()) < xRoomInfo.maxpalyer())
		{
			return false;
		}

		for (int i = 0; i < xPlayerList.size(); i++)
		{
			const NFGUID& xPlayerID = xPlayerList[i];
			if (xPlayerID.IsNull())
			{
				continue;
			}

			NFGUID xOldRoomID;
			if (!m_pPVPMatchRedisModule->GetPlayerRoomID(xPlayerID, xOldRoomID))
			{
				continue;
			}

			if (!xOldRoomID.IsNull())
			{
				continue;
			}

			if (!m_pPVPMatchRedisModule->SetPlayerRoomID(xPlayerID, xRoomID))
			{
				continue;
			}

			*xRoomInfo.add_xblueplayer() = NFINetModule::NFToPB(xPlayerID);
		}
	}

	UpdateRoomStatus(xRoomInfo);

	if (!m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	return true;
}

bool NFCPVPMatchModule::PlayerLeaveRoom(const NFGUID& self, const NFGUID& xRoomID)
{
	if (!xRoomID.IsNull())
	{
		return false;
	}

	NFMsg::PVPRoomInfo xRoomInfo;
	if (!m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	NFMsg::PVPRoomInfo xNewRoomInfo;
	xNewRoomInfo.CopyFrom(xRoomInfo);

	xNewRoomInfo.clear_xredplayer();
	for (int i = 0; i < xRoomInfo.xredplayer_size(); ++i)
	{
		if (NFINetModule::PBToNF(xRoomInfo.xredplayer(i)) == self)
		{
			continue;
		}

		xRoomInfo.add_xredplayer()->CopyFrom(xRoomInfo.xredplayer(i));
	}

	xNewRoomInfo.clear_xblueplayer();
	for (int i = 0; i < xRoomInfo.xblueplayer_size(); ++i)
	{
		if (NFINetModule::PBToNF(xRoomInfo.xblueplayer(i)) == self)
		{
			continue;
		}

		xRoomInfo.add_xredplayer()->CopyFrom(xRoomInfo.xblueplayer(i));
	}

	if (!m_pPVPMatchRedisModule->SetPlayerRoomID(self, NFGUID()))
	{
		return false;
	}

	UpdateRoomStatus(xRoomInfo);
	if (!m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	return true;
}

bool NFCPVPMatchModule::PlayerListLeaveRoom(const std::vector<NFGUID>& xPlayerList, const NFGUID& xRoomID)
{
	if (!xRoomID.IsNull())
	{
		return false;
	}

	NFMsg::PVPRoomInfo xRoomInfo;
	if (!m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	NFMsg::PVPRoomInfo xNewRoomInfo;
	xNewRoomInfo.CopyFrom(xRoomInfo);

	std::map<NFGUID, NFGUID> xMapPlayerList;
	for (int i = 0; i < xPlayerList.size(); i++)
	{
		const NFGUID xPlayerID = xPlayerList[i];
		xMapPlayerList[xPlayerID] = xPlayerID;
	}

	xNewRoomInfo.clear_xredplayer();
	for (int i = 0; i < xRoomInfo.xredplayer_size(); ++i)
	{
		std::map<NFGUID, NFGUID>::iterator iter = xMapPlayerList.find(NFINetModule::PBToNF(xRoomInfo.xredplayer(i)));
		if (iter != xMapPlayerList.end())
		{
			m_pPVPMatchRedisModule->SetPlayerRoomID(NFINetModule::PBToNF(xRoomInfo.xredplayer(i)), NFGUID());
			continue;
		}

		xRoomInfo.add_xredplayer()->CopyFrom(xRoomInfo.xredplayer(i));
	}

	xNewRoomInfo.clear_xblueplayer();
	for (int i = 0; i < xRoomInfo.xblueplayer_size(); ++i)
	{
		std::map<NFGUID, NFGUID>::iterator iter = xMapPlayerList.find(NFINetModule::PBToNF(xRoomInfo.xblueplayer(i)));
		if (iter != xMapPlayerList.end())
		{
			m_pPVPMatchRedisModule->SetPlayerRoomID(NFINetModule::PBToNF(xRoomInfo.xblueplayer(i)), NFGUID());
			continue;
		}

		xRoomInfo.add_xredplayer()->CopyFrom(xRoomInfo.xblueplayer(i));
	}

	{
		return false;
	}

	UpdateRoomStatus(xRoomInfo);
	if (!m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	return true;
}

int NFCPVPMatchModule::NeedPlayerCount(const NFMsg::PVPRoomInfo& xRoomInfo, const int nRedOrebulue)
{
	if (nRedOrebulue == EPVPREDORBLUE_RED)
	{
		return xRoomInfo.maxpalyer() - xRoomInfo.xredplayer_size();
	}
	else
	{
		return xRoomInfo.maxpalyer() - xRoomInfo.xblueplayer_size();
	}

	return 0;
}

bool NFCPVPMatchModule::UpdateRoomStatus(NFMsg::PVPRoomInfo& xRoomInfo, const int nTargetStatus/* = -1*/)
{
	int nNeedRedCount = NeedPlayerCount(xRoomInfo, EPVPREDORBLUE_RED);
	int nNeedBlueCount = NeedPlayerCount(xRoomInfo, EPVPREDORBLUE_BULE);

	if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_WAIT)
	{
		if (nNeedRedCount <= 0 && nNeedBlueCount <= 0)
		{
			xRoomInfo.set_ncellstatus(EPVPROOMSTATUS_WAITFIGHT);

			m_pPVPMatchRedisModule->DeleteStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAIT, NFINetModule::PBToNF(xRoomInfo.roomid()));
			m_pPVPMatchRedisModule->SetStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITFIGHT, NFINetModule::PBToNF(xRoomInfo.roomid()));
			return true;
		}
	}

	if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_WAITFIGHT)
	{
		if (nNeedRedCount > 0 && nNeedBlueCount > 0)
		{
			xRoomInfo.set_ncellstatus(EPVPROOMSTATUS_WAIT);

			m_pPVPMatchRedisModule->DeleteStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITFIGHT, NFINetModule::PBToNF(xRoomInfo.roomid()));
			m_pPVPMatchRedisModule->SetStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAIT, NFINetModule::PBToNF(xRoomInfo.roomid()));
			return true;
		}
	}

	switch (nTargetStatus)
	{
	case EPVPROOMSTATUS_WAITFIGHT:
	{
		if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_WAIT)
		{
			if (nNeedRedCount <= 0 && nNeedBlueCount <= 0)
			{
				xRoomInfo.set_ncellstatus(EPVPROOMSTATUS_WAITFIGHT);

				m_pPVPMatchRedisModule->DeleteStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAIT, NFINetModule::PBToNF(xRoomInfo.roomid()));
				m_pPVPMatchRedisModule->SetStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITFIGHT, NFINetModule::PBToNF(xRoomInfo.roomid()));
				return true;
			}
		}
	}
	break;
	case EPVPROOMSTATUS_WAITCREATEECTYPE:
	{
		if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_WAITFIGHT)
		{
			if (nNeedRedCount <= 0 && nNeedBlueCount <= 0)
			{
				xRoomInfo.set_ncellstatus(EPVPROOMSTATUS_WAITCREATEECTYPE);

				m_pPVPMatchRedisModule->DeleteStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITFIGHT, NFINetModule::PBToNF(xRoomInfo.roomid()));
				m_pPVPMatchRedisModule->SetStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITCREATEECTYPE, NFINetModule::PBToNF(xRoomInfo.roomid()));
				return true;
			}
		}
	}
	break;
	case EPVPROOMSTATUS_FIGHT:
	{
		if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_WAITCREATEECTYPE)
		{

			xRoomInfo.set_ncellstatus(EPVPROOMSTATUS_FIGHT);

			m_pPVPMatchRedisModule->DeleteStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_WAITCREATEECTYPE, NFINetModule::PBToNF(xRoomInfo.roomid()));
			m_pPVPMatchRedisModule->SetStatusRoomID(xRoomInfo.npvpmode(), xRoomInfo.npvpgrade(), EPVPROOMSTATUS_FIGHT, NFINetModule::PBToNF(xRoomInfo.roomid()));
			return true;
		}
	}
	break;
	case EPVPROOMSTATUS_FINISH:
	{

	}
	break;
	default:
		break;
	}

	return false;
}

void NFCPVPMatchModule::ProecessWaitRoom()
{
	for (int nPVPMode = 0; nPVPMode < EPVPMODE::EPVPMODE_THREE; nPVPMode++)
	{
		for (int nGrade = 0; nGrade < EPVPDEFINE_MAXGRADE; nGrade++)
		{
			std::vector<NFGUID> xRoomIDList;
			if (m_pPVPMatchRedisModule->GetStatusRoomID(nPVPMode, nGrade, EPVPROOMSTATUS_WAIT, xRoomIDList))
			{
				std::vector<NFMsg::PVPRoomInfo> vecRoomInfoList;
				if (vecRoomInfoList.size() > 0)
				{
					if (m_pPVPMatchRedisModule->GetRoomInfoList(xRoomIDList, vecRoomInfoList))
					{
						for (int i = 0; i < vecRoomInfoList.size(); i++)
						{
							const NFMsg::PVPRoomInfo& xRoomInfo = vecRoomInfoList[i];
							int nNeedRedCount = NeedPlayerCount(xRoomInfo, EPVPREDORBLUE_RED);
							int nNeedBlueCount = NeedPlayerCount(xRoomInfo, EPVPREDORBLUE_BULE);

							std::vector<NFGUID> xNeedRedPlayerList;
							if (m_pPVPMatchRedisModule->PopSinglePlayerList(nPVPMode, nGrade, nNeedRedCount, xNeedRedPlayerList))
							{
								continue;
							}

							NFGUID xRoomID = NFINetModule::PBToNF(xRoomInfo.roomid());
							PlayerListEnterRoom(xNeedRedPlayerList, EPVPREDORBLUE_RED, xRoomID);

							std::vector<NFGUID> xNeedBluePlayerList;
							if (m_pPVPMatchRedisModule->PopSinglePlayerList(nPVPMode, nGrade, nNeedBlueCount, xNeedBluePlayerList))
							{
								continue;
							}

							PlayerListEnterRoom(xNeedBluePlayerList, EPVPREDORBLUE_BULE, xRoomID);
						}
					}
				}				
			}
		}
	}
}

void NFCPVPMatchModule::ProecessRoomBeginFight()
{
	for (int nPVPMode = 0; nPVPMode < EPVPMODE::EPVPMODE_THREE; nPVPMode++)
	{
		for (int nGrade = 0; nGrade < EPVPDEFINE_MAXGRADE; nGrade++)
		{
			std::vector<NFGUID> xRoomIDList;
			if (m_pPVPMatchRedisModule->GetStatusRoomID(nPVPMode, nGrade, EPVPROOMSTATUS_WAITFIGHT, xRoomIDList))
			{
				std::vector<NFMsg::PVPRoomInfo> vecRoomInfoList;
				if (vecRoomInfoList.size() > 0)
				{
					if (m_pPVPMatchRedisModule->GetRoomInfoList(xRoomIDList, vecRoomInfoList))
					{
						for (int i = 0; i < vecRoomInfoList.size(); i++)
						{
							NFMsg::PVPRoomInfo& xRoomInfo = vecRoomInfoList[i];
							if (UpdateRoomStatus(xRoomInfo, EPVPROOMSTATUS_WAITCREATEECTYPE))
							{
								//send to Game Create ;

								//find a player
								for (int iPlayer = 0; iPlayer < xRoomInfo.xblueplayer_size(); iPlayer++)
								{
									NFGUID xPlayerID = NFINetModule::PBToNF(xRoomInfo.xblueplayer(iPlayer));
									int nGameID = m_pPlayerRedisModule->GetPlayerCacheGameID(xPlayerID);
									if (nGameID > 0)
									{
										NFMsg::ReqCreatePVPEctype xMsg;
										*xMsg.mutable_self_id() = NFINetModule::NFToPB(xPlayerID);
										xMsg.CopyFrom(xRoomInfo);

										if (m_pWorldNet_ServerModule->SendMsgToGame(nGameID, NFMsg::EGMI_REQ_CREATEPVPECTYPE, xMsg, xPlayerID))
										{
											break;
										}
									}
								}

								m_pPVPMatchRedisModule->SetRoomInfo(NFINetModule::PBToNF(xRoomInfo.roomid()), xRoomInfo);
							}
						}
					}
				}				
			}
		}
	}
}


void NFCPVPMatchModule::ProcessSingePlayerRoom()
{
	NFGUID xSelf;
	for (int nPVPMode = 0; nPVPMode < EPVPMODE::EPVPMODE_THREE; nPVPMode++)
	{
		for (int nGrade = 0; nGrade < EPVPDEFINE_MAXGRADE; nGrade++)
		{
			const int nHavePlayer = m_pPVPMatchRedisModule->GetSinglePlayerCount(xSelf, nPVPMode, nGrade);
			const int nNeedPlayer = 2 * GetMemberCount(nPVPMode);
			if (nNeedPlayer > nHavePlayer)
			{
				continue;;
			}

			std::vector<NFGUID> xRedPlayer;
			std::vector<NFGUID> xBluePlayer;

			m_pPVPMatchRedisModule->PopSinglePlayerList(nPVPMode, nGrade, GetMemberCount(nPVPMode), xRedPlayer);
			m_pPVPMatchRedisModule->PopSinglePlayerList(nPVPMode, nGrade, GetMemberCount(nPVPMode), xBluePlayer);

			NFGUID xRoomID = CreateRoom(xSelf, nPVPMode, nGrade);
			PlayerListEnterRoom(xRedPlayer, EPVPREDORBLUE_RED, xRoomID);
			PlayerListEnterRoom(xBluePlayer, EPVPREDORBLUE_BULE, xRoomID);
		}
	}
}

void NFCPVPMatchModule::OnReqPVPApplyMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqPVPApplyMatch);

	NFMsg::AckPVPApplyMatch xAck;
	*xAck.mutable_self_id() = xMsg.self_id();
	xAck.set_applytype(xMsg.applytype());
	xAck.set_nresult(0);

	switch (xMsg.applytype())
	{
	case NFMsg::ReqPVPApplyMatch::EApplyType_Single:
	{
		if (ApplyPVP(nPlayerID, xMsg.npvpmode(), xMsg.score()))
		{
			xAck.set_nresult(1);
		}

		NFGUID xRoomID;
		if (m_pPVPMatchRedisModule->GetPlayerRoomID(nPlayerID, xRoomID))
		{
			NFMsg::PVPRoomInfo xRoomInfo;
			if (m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
			{
				xAck.mutable_xroominfo()->CopyFrom(xRoomInfo);
			}
		}

		m_pWorldNet_ServerModule->GetNetModule()->SendMsgPB(NFMsg::EGMI_ACK_PVPAPPLYMACTCH, xAck, nSockIndex, nPlayerID);
	}
	break;
	case NFMsg::ReqPVPApplyMatch::EApplyType_Team:
	{
		if (xMsg.has_team_id())
		{
			NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
			if (!xTeamID.IsNull())
			{
				std::vector<NFGUID> xPlayerList;
				if (m_pTeamModule->GetMemberList(nPlayerID, xTeamID, xPlayerList))
				{
					NFCDataList varMember;
					for (size_t i = 0; i < xPlayerList.size(); i++)
					{
						varMember.AddObject(xPlayerList[i]);
					}

					if (TeamApplyPVP(xTeamID, varMember, xMsg.npvpmode(), xMsg.score()))
					{
						xAck.set_nresult(1);

						NFGUID xRoomID;
						if (m_pPVPMatchRedisModule->GetPlayerRoomID(nPlayerID, xRoomID))
						{
							NFMsg::PVPRoomInfo xRoomInfo;
							if (m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
							{
								xAck.mutable_xroominfo()->CopyFrom(xRoomInfo);
							}
						}

						m_pTeamModule->BroadcastMsgToTeam(nPlayerID, xTeamID, NFMsg::EGMI_ACK_PVPAPPLYMACTCH, xAck);

						
						return;
					}
				}
			}
		}

		m_pWorldNet_ServerModule->GetNetModule()->SendMsgPB(NFMsg::EGMI_ACK_PVPAPPLYMACTCH, xAck, nSockIndex, nPlayerID);

	}
	break;
	default:
		break;
	}
}

void NFCPVPMatchModule::OnAckCreatePVPEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::AckCreatePVPEctype);

	NFGUID xRoomID = NFINetModule::PBToNF(xMsg.xroominfo().roomid());
	if (!xRoomID.IsNull())
	{
		int nServerID = 0;
		int nSceneID = 0;
		int nGroup = 0;
		if (!xMsg.xroominfo().has_serverid())
		{
			return;
		}

		if (!xMsg.xroominfo().has_sceneid())
		{
			return;
		}
		if (!xMsg.xroominfo().has_groupid())
		{
			return;
		}

		nServerID = xMsg.xroominfo().serverid();
		nSceneID = xMsg.xroominfo().sceneid();
		nGroup = xMsg.xroominfo().groupid();

		NFMsg::PVPRoomInfo xRoomInfo;
		if (m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
		{
			UpdateRoomStatus(xRoomInfo, EPVPROOMSTATUS_FIGHT);

			if (xRoomInfo.ncellstatus() == EPVPROOMSTATUS_FIGHT)
			{
				xRoomInfo.set_serverid(nServerID);
				xRoomInfo.set_sceneid(nSceneID);
				xRoomInfo.set_groupid(nGroup);

				if (m_pPVPMatchRedisModule->SetRoomInfo(xRoomID, xRoomInfo))
				{
					BroadcastMsgToRoom(nPlayerID, xRoomID, NFMsg::EGMI_ACK_CREATEPVPECTYPE, xMsg);
				}
			}
		}
	}
}


bool NFCPVPMatchModule::BroadcastMsgToRoom(const NFGUID& self, const NFGUID& xRoomID, const uint16_t nMsgID, google::protobuf::Message& xData)
{
	std::vector<std::string> xPlayerList;
	std::vector<int64_t> xGameIDList;
	std::vector<NFGUID> xPlayerIDList;

	NFMsg::PVPRoomInfo xRoomInfo;
	if (!m_pPVPMatchRedisModule->GetRoomInfo(xRoomID, xRoomInfo))
	{
		return false;
	}

	for (int i = 0; i < xRoomInfo.xblueplayer_size(); i++)
	{
		xPlayerIDList.push_back(NFINetModule::PBToNF(xRoomInfo.xblueplayer(i)));
	}

	for (int i = 0; i < xRoomInfo.xredplayer_size(); i++)
	{
		xPlayerIDList.push_back(NFINetModule::PBToNF(xRoomInfo.xredplayer(i)));
	}

	for (int i = 0; i < xPlayerIDList.size(); i++)
	{
		xPlayerList.push_back(xPlayerIDList[i].ToString());
	}

	if (!m_pPlayerRedisModule->GetPlayerCacheGameID(xPlayerList, xGameIDList))
	{
		return false;
	}

	for (int i = 0; i < xGameIDList.size() && i < xPlayerList.size(); i++)
	{
		int nGameID = xGameIDList[i];
		NFGUID xPlayer;
		xPlayer.FromString(xPlayerList[i]);
		m_pWorldNet_ServerModule->SendMsgToGame(nGameID, (NFMsg::EGameMsgID)nMsgID, xData, xPlayer);
	}

	return true;
}
