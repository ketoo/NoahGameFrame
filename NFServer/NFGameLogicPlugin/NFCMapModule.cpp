// -------------------------------------------------------------------------
//    @FileName				:    NFCMapModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCMapModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCMapModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCMapModule::Init()
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

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_BIG_MAP_INFO, this, &NFCMapModule::ReqBigMapsInfo)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_INFO, this, &NFCMapModule::ReqMapTitleInfo)) { return false; }

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_HOLD_MAP_GRID, this, &NFCMapModule::ReqStation)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_GET_MAP_GRID_AWARD, this, &NFCMapModule::ReqGetMapAward)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_LEAVE_MSG_MAP_GRID, this, &NFCMapModule::ReqLeaveMsgToMap)) { return false; }

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_HUNTING, this, &NFCMapModule::ReqMapHunting)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_KING_WAR, this, &NFCMapModule::ReqMapKingWar)) { return false; }

    return true;
}

void NFCMapModule::ReqBigMapsInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqBigMapInfo);


	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement("Map");
	
	//find all title info
	NFMsg::AckBigMapInfo xAckBigMapInfo;
	std::vector<NFMsg::BigMapGridBaseInfo> xBigMapGridBaseInfoList;
	if (m_pBigMapRedisModule->GetGridBaseInfo(xBigMapGridBaseInfoList))
	{
		for (std::vector<NFMsg::BigMapGridBaseInfo>::iterator it = xBigMapGridBaseInfoList.begin();
		it != xBigMapGridBaseInfoList.end(); ++it)
		{
			NFMsg::BigMapGridBaseInfo* pBigMapGridBaseInfo = xAckBigMapInfo.add_grid_base_info();
			if (pBigMapGridBaseInfo)
			{
				pBigMapGridBaseInfo->CopyFrom(*it);
			}
		}

		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_BIG_MAP_INFO, xAckBigMapInfo, nPlayerID);
	}
}

void NFCMapModule::ReqMapTitleInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqBigMapGridInfo);
	
	NFMsg::AckBigMapGridInfo xAckBigMapGridInfo;

	int nTitleCount = xMsg.map_title_id_size();
	for (int i = 0; i < nTitleCount; ++i)
	{
		std::string strTitleID = xMsg.map_title_id(i);

		if (!m_pElementModule->ExistElement(strTitleID))
		{
			continue;
		}

		NFMsg::BigMapGridBaseInfo xGridBaseInfo;
		if (!m_pBigMapRedisModule->GetGridBaseInfo(strTitleID, xGridBaseInfo))
		{
			continue;
		}

		std::vector<NFMsg::BigMapLeaveMsg> xLeaveMsgList;
		if (!m_pBigMapRedisModule->GetGridLeaveMsgInfo(strTitleID, xLeaveMsgList))
		{
			continue;
		}

		std::vector<NFMsg::BigMapWarHistory> xWarHistoryList;
		if (!m_pBigMapRedisModule->GetGridWarHistoryInfo(strTitleID, xWarHistoryList))
		{
			continue;
		}

		NFMsg::BigMapGridDetailInfo* pBigMapGridDetailInfo = xAckBigMapGridInfo.add_map_data();
		if (pBigMapGridDetailInfo)
		{
			pBigMapGridDetailInfo->mutable_grid_base_info()->CopyFrom(xGridBaseInfo);
		}

		std::vector<NFMsg::BigMapLeaveMsg>::iterator itLeaveMsg = xLeaveMsgList.begin();
		for (itLeaveMsg; itLeaveMsg != xLeaveMsgList.end(); ++itLeaveMsg)
		{
			NFMsg::BigMapLeaveMsg* pBigMapLeaveMsg = pBigMapGridDetailInfo->add_leave_msg();
			if (pBigMapLeaveMsg)
			{
				pBigMapLeaveMsg->CopyFrom(*itLeaveMsg);
			}
		}

		std::vector<NFMsg::BigMapWarHistory>::iterator itWar = xWarHistoryList.begin();
		for (itWar; itWar != xWarHistoryList.end(); ++itWar)
		{
			NFMsg::BigMapWarHistory* pBigMapWarHistory = pBigMapGridDetailInfo->add_war_history();
			if (pBigMapWarHistory)
			{
				pBigMapWarHistory->CopyFrom(*itWar);
			}
		}
	}

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_MAP_GRID_INFO, xAckBigMapGridInfo, nPlayerID);
}

void NFCMapModule::ReqStation(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqHoldMapGrid);

	if (!m_pElementModule->ExistElement(xMsg.map_title_id()))
	{
		return;
	}
	/*
	NFGUID xGuildID = NFINetModule::PBToNF(xMsg.guild_id());
	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pGuildRedisModule->GetGuildCachePropertyInfo(xGuildID);
	if (!xPropertyManager)
	{
		return;
	}
	//remove form last grid

	int nResourece = xPropertyManager->GetPropertyInt(NFrame::Guild::KingWarResource());
	m_pBigMapRedisModule->AddGridStationInfo(xMsg.map_title_id(), xGuildID, nResourece);
	*/

}

void NFCMapModule::ReqGetMapAward(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqGetMapAward);

	if (!m_pElementModule->ExistElement(xMsg.map_title_id()))
	{
		return;
	}

	//get guild informatin and send mail to all members to send award item
}

void NFCMapModule::ReqLeaveMsgToMap(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqLeaveMapMsg);

	if (!m_pElementModule->ExistElement(xMsg.map_title_id()))
	{
		return;
	}

	m_pBigMapRedisModule->AddGridLeaveMsgInfo(xMsg.map_title_id(), xMsg.leave_msg());
}

void NFCMapModule::ReqMapHunting(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqMapHunting);

	if (!m_pElementModule->ExistElement(xMsg.map_title_id()))
	{
		return;
	}

	NFMsg::BigMapGridBaseInfo xGridBaseInfo;
	if (!m_pBigMapRedisModule->GetGridBaseInfo(xMsg.map_title_id(), xGridBaseInfo))
	{
		return;
	}

	xGridBaseInfo.set_hurting_time(pPluginManager->GetNowTime());
	xGridBaseInfo.mutable_hurter()->CopyFrom(NFINetModule::NFToPB(nPlayerID));

	m_pBigMapRedisModule->SetGridBaseInfo(xMsg.map_title_id(), xGridBaseInfo);

	//show msgbox
}

void NFCMapModule::ReqMapKingWar(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqMapKingWar);

	if (!m_pElementModule->ExistElement(xMsg.map_title_id()))
	{
		return;
	}

	NFMsg::BigMapGridBaseInfo xGridBaseInfo;
	if (!m_pBigMapRedisModule->GetGridBaseInfo(xMsg.map_title_id(), xGridBaseInfo))
	{
		return;
	}

	xGridBaseInfo.set_kingwar_time(pPluginManager->GetNowTime());
	xGridBaseInfo.mutable_kingwarrer()->CopyFrom(NFINetModule::NFToPB(nPlayerID));

	m_pBigMapRedisModule->SetGridBaseInfo(xMsg.map_title_id(), xGridBaseInfo);

	//show msgbox
}

void NFCMapModule::EndMapHunting(const std::string& strTitleID)
{
	NFMsg::BigMapGridBaseInfo xGridBaseInfo;
	if (!m_pBigMapRedisModule->GetGridBaseInfo(strTitleID, xGridBaseInfo))
	{
		return;
	}

	xGridBaseInfo.set_hurting_time(0);
	xGridBaseInfo.mutable_kingwarrer()->CopyFrom(NFINetModule::NFToPB(NFGUID()));

	m_pBigMapRedisModule->SetGridBaseInfo(strTitleID, xGridBaseInfo);

	//show msgbox and send mail to award
}

void NFCMapModule::EndMapKingWar(const std::string& strTitleID)
{
	NFMsg::BigMapGridBaseInfo xGridBaseInfo;
	if (!m_pBigMapRedisModule->GetGridBaseInfo(strTitleID, xGridBaseInfo))
	{
		return;
	}

	xGridBaseInfo.set_kingwar_time(0);
	xGridBaseInfo.mutable_kingwarrer()->CopyFrom(NFINetModule::NFToPB(NFGUID()));

	m_pBigMapRedisModule->SetGridBaseInfo(strTitleID, xGridBaseInfo);

	//show msgbox and send mail to award
}

void NFCMapModule::SetKingForGrid(const std::string& strTitleID, const NFGUID& xGuildID)
{
	/*
	if (!m_pElementModule->ExistElement(strTitleID))
	{
		return;
	}

	NFMsg::BigMapGridBaseInfo xGridBaseInfo;
	if (!m_pBigMapRedisModule->GetGridBaseInfo(strTitleID, xGridBaseInfo))
	{
		return;
	}

	//get all guild information to set in grid base info
	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pGuildRedisModule->GetGuildCachePropertyInfo(xGuildID);
	const NFGUID xGUID = xPropertyManager->GetPropertyObject(NFrame::Guild::GuildID());
	const std::string& strIcon = xPropertyManager->GetPropertyString(NFrame::Guild::GuildIcon());
	const int nLevel = xPropertyManager->GetPropertyInt(NFrame::Guild::GuildLevel());
	const int nMemberCount = xPropertyManager->GetPropertyInt(NFrame::Guild::GuildMemeberCount());
	const int nResource = xPropertyManager->GetPropertyInt(NFrame::Guild::KingWarResource());

	xGridBaseInfo.mutable_guild_info()->mutable_id()->CopyFrom(NFINetModule::NFToPB(xGUID));
	xGridBaseInfo.mutable_guild_info()->set_level(nLevel);
	xGridBaseInfo.mutable_guild_info()->set_count(nMemberCount);
	xGridBaseInfo.mutable_guild_info()->set_icon(strIcon);
	xGridBaseInfo.mutable_guild_info()->set_resource(nResource);

	m_pBigMapRedisModule->SetGridBaseInfo(strTitleID, xGridBaseInfo);
	*/
}

void NFCMapModule::LeaveStation(const std::string& strTitleID, const NFGUID& xGuildID)
{

}

