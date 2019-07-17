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

#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFGamePVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFGamePVPModule::Init()
{
	
    return true;
}


bool NFGamePVPModule::Shut()
{
    return true;
}

bool NFGamePVPModule::Execute()
{
    
    return true;
}

bool NFGamePVPModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFGamePVPModule::OnNPCClassEvent);
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFGamePVPModule::OnPlayerClassEvent);

	m_pSceneModule->AddEnterSceneConditionCallBack(this, &NFGamePVPModule::EnterSceneConditionEvent);

	m_pSceneModule->AddBeforeEnterSceneGroupCallBack(this, &NFGamePVPModule::BeforeEnterSceneGroupEvent);
	m_pSceneModule->AddAfterEnterSceneGroupCallBack(this, &NFGamePVPModule::AfterEnterSceneGroupEvent);
	m_pSceneModule->AddBeforeLeaveSceneGroupCallBack(this, &NFGamePVPModule::BeforeLeaveSceneGroupEvent);
	m_pSceneModule->AddAfterLeaveSceneGroupCallBack(this, &NFGamePVPModule::AfterLeaveSceneGroupEvent);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFGamePVPModule::OnReqSearchOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CANCEL_SEARCH, this, &NFGamePVPModule::OnReqCancelSearchProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_END_OPPNENT, this, &NFGamePVPModule::OnReqEndPVPOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_HOME_SCENE, this, &NFGamePVPModule::OnReqSwapHomeSceneProcess)) { return false; }

	if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_SEARCH_OPPNENT, this, &NFGamePVPModule::OnAckSearchOpponentProcess)) { return false; }
	if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_CANCEL_SEARCH, this, &NFGamePVPModule::OnAckCancelSearchProcess)) { return false; }

	m_pSceneModule->AddGroupPropertyCallBack(NFrame::Group::MatchOpponentK(), this, &NFGamePVPModule::OnMatchKillsEvent);
	m_pSceneModule->AddGroupPropertyCallBack(NFrame::Group::MatchOpponentD(), this, &NFGamePVPModule::OnMatchDeathsEvent);

    return true;
}

bool NFGamePVPModule::ReadyExecute()
{

	return false;
}

void NFGamePVPModule::OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);
	for (int i = 0; i < xMsg.friends_size(); ++i)
	{
		NFGUID id = NFINetModule::PBToNF(xMsg.friends(i));
		if (!id.IsNull() && !m_pKernelModule->ExistObject(id))
		{
			//error
			m_pLogModule->LogError("error for " + id.ToString() + " not in this game server, invited by " + nPlayerID.ToString());
			return;
		}

		const int diamond = m_pKernelModule->GetPropertyInt(id, NFrame::Player::Diamond()) - xMsg.diamond();
		if (diamond >= 0)
		{
			//m_pKernelModule->SetPropertyInt(id, NFrame::Player::Diamond(), diamond);
		}
		else 
		{
			m_pLogModule->LogError("error for " + id.ToString() + " no enough money, invited by " + nPlayerID.ToString());
			return;
		}
	}

	const int diamond = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::Diamond()) - xMsg.diamond();
	if (diamond >= 0)
	{
		//m_pKernelModule->SetPropertyInt(id, NFrame::Player::Diamond(), diamond);
	}
	else
	{
		m_pLogModule->LogError("error for " + nPlayerID.ToString() + " no enough money, invited by " + nPlayerID.ToString());
		return;
	}


	const int nSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());
	const NFGUID xWarID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
	if (xWarID.IsNull())
	{
		int nHomeSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::HomeSceneID());
		xMsg.set_self_scene(nHomeSceneID);
		int nHasKey = nPlayerID.nData64;
		//check all friends in the same game server
		m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, nMsgID, xMsg, nPlayerID);
	}

	std::ostringstream stream;
	stream << nPlayerID.ToString() << " want to join a start a match, the match id now is:" << xWarID.ToString();
	m_pLogModule->LogInfo(stream);
}

void NFGamePVPModule::OnReqCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckCancelSearch);
	*xMsg.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);

	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.nData64, nMsgID, xMsg, nPlayerID);
}

void NFGamePVPModule::OnAckSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::AckSearchOppnent);
	if (xMsg.team_members_size() <= 0)
	{
		m_pLogModule->LogError("error for size == 0");
		return;
	}

	std::ostringstream stream;

	const int sceneID = xMsg.scene_id();
	const int groupID = m_pKernelModule->RequestGroupScene(xMsg.scene_id());
	const NFVector3& pos = m_pSceneModule->GetRelivePosition(xMsg.scene_id(), 0);
	const NFGUID matchID = m_pKernelModule->CreateGUID();

	stream << " A match now start, the match id now is:" << matchID.ToString() << " team's id is:" << NFINetModule::PBToNF(xMsg.team_id()).ToString();
	stream << " sceneID:" << sceneID << " groupID:" << groupID;

	for (int i = 0; i < xMsg.team_members_size(); ++i)
	{
		NFGUID id = NFINetModule::PBToNF(xMsg.team_members(i));
		stream << " member:" << i << " " << id.ToString();

		if (!m_pKernelModule->ExistObject(id))
		{
			stream << " some one(team member) not in this game server:" << id.ToString();
			m_pLogModule->LogError(stream);
			return;
		}
	}

	const NFMsg::PVPPlayerInfo& pvpPlayerInfo = xMsg.opponent();

	stream << " MatchOpponentID:" << NFINetModule::PBToNF(pvpPlayerInfo.id()).ToString();
	stream << " MatchGambleDiamond:" << xMsg.gamble_diamond();
	stream << " MatchOpponentLevel:" << pvpPlayerInfo.level();
	stream << " MatchOpponentCup:" << pvpPlayerInfo.battle_point();
	stream << " MatchOpponentName:" << pvpPlayerInfo.name();

	stream << " MatchOpponentHeroID1:" << NFINetModule::PBToNF(pvpPlayerInfo.hero_id1()).ToString();
	stream << " MatchOpponentHeroID2:" << NFINetModule::PBToNF(pvpPlayerInfo.hero_id2()).ToString();
	stream << " MatchOpponentHeroID3:" << NFINetModule::PBToNF(pvpPlayerInfo.hero_id3()).ToString();
	stream << " MatchOpponentHeroCnf1:" << pvpPlayerInfo.hero_cnf1();
	stream << " MatchOpponentHeroCnf2:" << pvpPlayerInfo.hero_cnf2();
	stream << " MatchOpponentHeroCnf3:" << pvpPlayerInfo.hero_cnf3();
	stream << " MatchOpponentHeroStar1:" << pvpPlayerInfo.hero_star1();
	stream << " MatchOpponentHeroStar2:" << pvpPlayerInfo.hero_star2();
	stream << " MatchOpponentHeroStar3:" << pvpPlayerInfo.hero_star3();
	m_pLogModule->LogInfo(stream);

	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchID(), matchID);
	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchTeamID(), NFINetModule::PBToNF(xMsg.team_id()));
	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentID(), NFINetModule::PBToNF(pvpPlayerInfo.id()));

	//m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::WarEventTime(), 0);
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchStar(), 0);
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchGambleDiamond(), xMsg.gamble_diamond());

	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentGold(), pvpPlayerInfo.gold());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentDiamond(), pvpPlayerInfo.diamond());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentLevel(), pvpPlayerInfo.level());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentCup(), pvpPlayerInfo.battle_point());
	m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentName(), pvpPlayerInfo.name());
	m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHead(), pvpPlayerInfo.head());

	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID1(), NFINetModule::PBToNF(pvpPlayerInfo.hero_id1()));
	m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf1(), pvpPlayerInfo.hero_cnf1());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar1(), pvpPlayerInfo.hero_star1());
	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID2(), NFINetModule::PBToNF(pvpPlayerInfo.hero_id2()));
	m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf2(), pvpPlayerInfo.hero_cnf2());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar2(), pvpPlayerInfo.hero_star2());
	m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID3(), NFINetModule::PBToNF(pvpPlayerInfo.hero_id3()));
	m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf3(), pvpPlayerInfo.hero_cnf3());
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar3(), pvpPlayerInfo.hero_star3());

	/*
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem1UsedCount(), 0);
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem2UsedCount(), 0);
	m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem3UsedCount(), 0);
	*/

	NF_SHARE_PTR<NFIRecord> pMatchMemberRecord = m_pSceneModule->FindRecord(sceneID, groupID, NFrame::Group::MatchMember::ThisName());

	for (int i = 0; i < xMsg.team_members_size(); ++i)
	{
		const NFGUID id = NFINetModule::PBToNF(xMsg.team_members(i));
		const std::string& strName = m_pKernelModule->GetPropertyString(id, NFrame::Player::Name());
		const std::string& strHeroCnf1 = m_pKernelModule->GetPropertyString(id, NFrame::Player::HeroCnfID1());
		const std::string& strHeroCnf2 = m_pKernelModule->GetPropertyString(id, NFrame::Player::HeroCnfID2());
		const std::string& strHeroCnf3 = m_pKernelModule->GetPropertyString(id, NFrame::Player::HeroCnfID3());

		const int nStar1 = m_pKernelModule->GetPropertyInt(id, NFrame::Player::HeroStar1());
		const int nStar2 = m_pKernelModule->GetPropertyInt(id, NFrame::Player::HeroStar2());
		const int nStar3 = m_pKernelModule->GetPropertyInt(id, NFrame::Player::HeroStar3());

		NF_SHARE_PTR<NFDataList> xRowData = pMatchMemberRecord->GetInitData();
		xRowData->SetObject(NFrame::Group::MatchMember::GUID, id);
		xRowData->SetString(NFrame::Group::MatchMember::Name, strName);
		xRowData->SetInt(NFrame::Group::MatchMember::K, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::D, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::A, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::Diamond, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::Cup, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::MVP, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::Streak, 0);
		xRowData->SetString(NFrame::Group::MatchMember::HeroCnf1, strHeroCnf1);
		xRowData->SetString(NFrame::Group::MatchMember::HeroCnf2, strHeroCnf2);
		xRowData->SetString(NFrame::Group::MatchMember::HeroCnf3, strHeroCnf3);
		xRowData->SetInt(NFrame::Group::MatchMember::HeroStar1, nStar1);
		xRowData->SetInt(NFrame::Group::MatchMember::HeroStar2, nStar2);
		xRowData->SetInt(NFrame::Group::MatchMember::HeroStar3, nStar3);
		xRowData->SetInt(NFrame::Group::MatchMember::HP1, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::HP2, 0);
		xRowData->SetInt(NFrame::Group::MatchMember::HP3, 0);

		pMatchMemberRecord->AddRow(-1, *xRowData);

		
		/* set opponent's hero data */

		/* set opponent's hero data */

		const NFGUID& xHeroID = m_pKernelModule->GetPropertyObject(id, NFrame::Player::HeroID1());
		m_pHeroModule->SwitchFightHero(id, xHeroID);

		m_pSceneProcessModule->RequestEnterScene(id, xMsg.scene_id(), groupID, 0, pos, NFDataList());

		m_pGameServerNet_ServerModule->SendMsgToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, std::string(msg, nLen), id);
	}

	NFGUID aiOnwer = NFINetModule::PBToNF(xMsg.team_members(0));

	NFDataList xHeroData1;
	xHeroData1.AddString(NFrame::NPC::Position());
	xHeroData1.AddVector3(pos);
	xHeroData1.AddString(NFrame::NPC::MasterID());
	xHeroData1.AddObject(NFINetModule::PBToNF(xMsg.opponent().id()));
	xHeroData1.AddString(NFrame::NPC::AIOwnerID());
	xHeroData1.AddObject(aiOnwer);
	xHeroData1.AddString(NFrame::NPC::NPCType());
	xHeroData1.AddInt(NFMsg::ENPCType::ENPCTYPE_HERO);
	xHeroData1.AddString(NFrame::NPC::Level());
	xHeroData1.AddInt(xMsg.opponent().level());
	xHeroData1.AddString(NFrame::NPC::HeroStar());
	xHeroData1.AddInt(xMsg.opponent().hero_star1());

	NFDataList xHeroData2;

	xHeroData2.AddString(NFrame::NPC::Position());
	xHeroData2.AddVector3(pos);
	xHeroData2.AddString(NFrame::NPC::MasterID());
	xHeroData2.AddObject(NFINetModule::PBToNF(xMsg.opponent().id()));
	xHeroData2.AddString(NFrame::NPC::AIOwnerID());
	xHeroData2.AddObject(aiOnwer);
	xHeroData2.AddString(NFrame::NPC::NPCType());
	xHeroData2.AddInt(NFMsg::ENPCType::ENPCTYPE_HERO);
	xHeroData2.AddString(NFrame::NPC::Level());
	xHeroData2.AddInt(xMsg.opponent().level());
	xHeroData2.AddString(NFrame::NPC::HeroStar());
	xHeroData2.AddInt(xMsg.opponent().hero_star2());

	NFDataList xHeroData3;
	xHeroData3.AddString(NFrame::NPC::Position());
	xHeroData3.AddVector3(pos);
	xHeroData3.AddString(NFrame::NPC::MasterID());
	xHeroData3.AddObject(NFINetModule::PBToNF((xMsg.opponent().id())));
	xHeroData3.AddString(NFrame::NPC::AIOwnerID());
	xHeroData3.AddObject(aiOnwer);
	xHeroData3.AddString(NFrame::NPC::NPCType());
	xHeroData3.AddInt(NFMsg::ENPCType::ENPCTYPE_HERO);
	xHeroData3.AddString(NFrame::NPC::Level());
	xHeroData3.AddInt(xMsg.opponent().level());
	xHeroData3.AddString(NFrame::NPC::HeroStar());
	xHeroData3.AddInt(xMsg.opponent().hero_star3());

	m_pKernelModule->CreateObject(NFINetModule::PBToNF(xMsg.opponent().hero_id1()), xMsg.scene_id(), groupID, NFrame::NPC::ThisName(), xMsg.opponent().hero_cnf1(), xHeroData1);
	m_pKernelModule->CreateObject(NFINetModule::PBToNF(xMsg.opponent().hero_id2()), xMsg.scene_id(), groupID, NFrame::NPC::ThisName(), xMsg.opponent().hero_cnf2(), xHeroData2);
	m_pKernelModule->CreateObject(NFINetModule::PBToNF(xMsg.opponent().hero_id3()), xMsg.scene_id(), groupID, NFrame::NPC::ThisName(), xMsg.opponent().hero_cnf3(), xHeroData3);

	for (int i = 0; i < xMsg.buildings_size(); ++i)
	{
		const NFMsg::ReqAddSceneBuilding& xBuilding = xMsg.buildings(i);

		const NFGUID xBuildingID = NFINetModule::PBToNF(xBuilding.guid());
		const NFGUID xMasterID = NFINetModule::PBToNF(xBuilding.master());
		const NFVector3 vPos = NFINetModule::PBToNF(xBuilding.pos());
		const std::string& strCnfID = xBuilding.config_id();
		const std::string& strMasterName = xBuilding.master_name();
		const int nSceneID = xBuilding.scene_id();

		NFDataList xDataArg;
		xDataArg.AddString(NFrame::NPC::Position());
		xDataArg.AddVector3(vPos);
		xDataArg.AddString(NFrame::NPC::MasterID());
		xDataArg.AddObject(xMasterID);
		xDataArg.AddString(NFrame::NPC::AIOwnerID());
		xDataArg.AddObject(aiOnwer);
		xDataArg.AddString(NFrame::NPC::NPCType());
		xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

		m_pKernelModule->CreateObject(xBuildingID, nSceneID, groupID, NFrame::NPC::ThisName(), strCnfID, xDataArg);
	}

}

void NFGamePVPModule::OnAckCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckCancelSearch);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFGamePVPModule::OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckHomeScene);

	int nSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::SceneID());
	int nGroupID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::GroupID());
	NFGUID warID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
	if (!warID.IsNull())
	{
		//fighting
		return;
	}

	int nHomeSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::HomeSceneID());
	const NFVector3& pos = m_pSceneModule->GetRelivePosition(nHomeSceneID, 0);
	m_pSceneProcessModule->RequestEnterScene(nPlayerID, nHomeSceneID, -1, 0, pos, NFDataList());
}

void NFGamePVPModule::OnReqEndPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
											  const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqEndBattle);

	//get oppnent
	//calculate how many monster has been killed
	//calculate how many building has been destroyed
	//calculate how much experice and how many money

	//tell client the end information
	//set oppnent 0
	EndTheBattle(nPlayerID, xMsg.auto_end());
}

int NFGamePVPModule::EnterSceneConditionEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFGamePVPModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFGamePVPModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//create building if the player back home
	//create building if the player wants attack the others

	//ResetPVPData(self);
	m_pHeroModule->ReliveAllHero(self);
	return 0;
}

int NFGamePVPModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SCENE_SINGLE_CLONE)
	{
	}
	else if (eSceneType == NFMsg::ESceneType::SCENE_MULTI_CLONE)
	{
	}
	else if (eSceneType == NFMsg::ESceneType::SCENE_HOME)
	{
		ResetPVPData(self);
	}
	
	return 0;
}

int NFGamePVPModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

void NFGamePVPModule::ResetPVPData(const NFGUID & self)
{
	const int sceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
	
	if (!m_pSceneModule->GetPropertyObject(sceneID, groupID, NFrame::Group::MatchID()).IsNull())
	{
		int playerCount = 0;
		NF_SHARE_PTR<NFIRecord> record = m_pSceneModule->FindRecord(sceneID, groupID, NFrame::Group::MatchMember::ThisName());
		if (record)
		{
			for (int i = 0; i < record->GetRows(); ++i)
			{
				if (record->IsUsed(i))
				{
					const NFGUID playerID = record->GetObject(i, NFrame::Group::MatchMember::GUID);
					const int playerSceneID = m_pKernelModule->GetPropertyInt32(playerID, NFrame::Player::SceneID());
					const int playerGroupID = m_pKernelModule->GetPropertyInt32(playerID, NFrame::Player::GroupID());
					if (playerSceneID == sceneID
						&& playerGroupID == groupID)
					{
						playerCount++;
					}
				}
			}

			if (playerCount <= 1)
			{
				record->Clear();


				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchID(), NFGUID());
				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchTeamID(), NFGUID());
				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentID(), NFGUID());

				//m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::WarEventTime(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchStar(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchGambleDiamond(), 0);

				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentGold(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentDiamond(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentLevel(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentCup(), 0);
				m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentName(), "");
				m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHead(), "");

				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID1(), NFGUID());
				m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf1(), "");
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar1(), 0);
				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID2(), NFGUID());
				m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf2(), "");
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar2(), 0);
				m_pSceneModule->SetPropertyObject(sceneID, groupID, NFrame::Group::MatchOpponentHeroID3(), NFGUID());
				m_pSceneModule->SetPropertyString(sceneID, groupID, NFrame::Group::MatchOpponentHeroCnf3(), "");
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchOpponentHeroStar3(), 0);

				/*
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem1UsedCount(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem2UsedCount(), 0);
				m_pSceneModule->SetPropertyInt(sceneID, groupID, NFrame::Group::MatchItem3UsedCount(), 0);
				*/

			}
		}
	}
}

void NFGamePVPModule::RecordPVPData(const NFGUID & self, const int nStar, const int nGold, const int nDiamond)
{
	//settlement information
	//NFMsg::AckEndBattle
	//how to record this war for these two people
	/*
	
	NFGUID xWarID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::MatchID());

	NF_SHARE_PTR<NFIRecord> xAttackWarRecord = m_pKernelModule->FindRecord(self, NFrame::Player::WarList::ThisName());

	NF_SHARE_PTR<NFDataList> xDataList = xAttackWarRecord->GetInitData();
	xDataList->SetObject(NFrame::Player::WarList::WarID, xWarID);
	//record the war information
	//the fields below need to be setted run-time
	static const std::string& BuildingCount() { static std::string x = "BuildingCOunt"; return x; };// int
	static const std::string& DestroiedBuilding() { static std::string x = "DestroiedBuilding"; return x; };// int

	static const std::string& DeadHero1() { static std::string x = "DeadHero1"; return x; };// int
	static const std::string& DeadHero2() { static std::string x = "DeadHero2"; return x; };// int
	static const std::string& DeadHero3() { static std::string x = "DeadHero3"; return x; };// int

	static const std::string& KilledHero1() { static std::string x = "KilledHero1"; return x; };// int
	static const std::string& KilledHero2() { static std::string x = "KilledHero2"; return x; };// int
	static const std::string& KilledHero3() { static std::string x = "KilledHero3"; return x; };// int

	static const std::string& KilledMasterCount() { static std::string x = "KilledMasterCount"; return x; };// int
	static const std::string& MasterCount() { static std::string x = "MasterCount"; return x; };// int

	xDataList->SetString(NFrame::Player::WarList::AttackerName, m_pKernelModule->GetPropertyString(self, NFrame::Player::Name()));
	xDataList->SetInt(NFrame::Player::WarList::AttackerCup, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Cup()));
	//xDataList->SetString(NFrame::Player::WarList::AttackerHero1, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos1CnfID()));
	//xDataList->SetInt(NFrame::Player::WarList::AttackerHero1Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos1Star()));
	//xDataList->SetString(NFrame::Player::WarList::AttackerHero2, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos2CnfID()));
	//xDataList->SetInt(NFrame::Player::WarList::AttackerHero2Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos2Star()));
	//xDataList->SetString(NFrame::Player::WarList::AttackerHero3, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos3CnfID()));
	//xDataList->SetInt(NFrame::Player::WarList::AttackerHero3Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos3Star()));
	xDataList->SetObject(NFrame::Player::WarList::AttackerID, self);
	xDataList->SetInt(NFrame::Player::WarList::AttackerLevel, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()));

	xDataList->SetString(NFrame::Player::WarList::BeAttackerName, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentName()));
	xDataList->SetInt(NFrame::Player::WarList::BeAttackerCup, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentCup()));
	//xDataList->SetString(NFrame::Player::WarList::BeAttackerHero1, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero1()));
	//xDataList->SetInt(NFrame::Player::WarList::BeAttackerHero1Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero1Star()));
	//xDataList->SetString(NFrame::Player::WarList::BeAttackerHero2, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero2()));
	//xDataList->SetInt(NFrame::Player::WarList::BeAttackerHero2Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero2Star()));
	//xDataList->SetString(NFrame::Player::WarList::BeAttackerHero3, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero3()));
	//xDataList->SetInt(NFrame::Player::WarList::BeAttackerHero3Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero3Star()));
	xDataList->SetObject(NFrame::Player::WarList::BeAttackerID, m_pKernelModule->GetPropertyObject(self, NFrame::Player::OpponentID()));
	xDataList->SetInt(NFrame::Player::WarList::BeAttackerLevel, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()));

	//xDataList->SetString(NFrame::Player::WarList::Item1, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item1()));
	//xDataList->SetInt(NFrame::Player::WarList::Item1Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item1UsedCount()));
	//xDataList->SetString(NFrame::Player::WarList::Item2, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item2()));
	//xDataList->SetInt(NFrame::Player::WarList::Item2Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item2UsedCount()));
	//xDataList->SetString(NFrame::Player::WarList::Item3, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item3()));
	//xDataList->SetInt(NFrame::Player::WarList::Item3Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item3UsedCount()));


	int64_t nEventTime = m_pKernelModule->GetPropertyInt(self, NFrame::Player::WarEventTime());
	int64_t nCostTime = NFGetTimeMS() - nEventTime;

	xDataList->SetInt(NFrame::Player::WarList::EventTime, nEventTime);
	xDataList->SetInt(NFrame::Player::WarList::WarStar, nStar);
	xDataList->SetInt(NFrame::Player::WarList::CostTime, nCostTime);
	xDataList->SetInt(NFrame::Player::WarList::Gold, nGold);
	xDataList->SetInt(NFrame::Player::WarList::Diamond, nDiamond);

	if (nStar >= 2)
	{
		xDataList->SetObject(NFrame::Player::WarList::Winner, self);
	}
	else
	{
		xDataList->SetObject(NFrame::Player::WarList::Winner, m_pKernelModule->GetPropertyObject(self, NFrame::Player::OpponentID()));
	}

	xAttackWarRecord->AddRow(-1, *xDataList);
	*/

	////////for beattacker////////////////////////////////////



}

int NFGamePVPModule::OnNPCClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFGamePVPModule::OnNPCHPEvent);
	}
	return 0;
}

int NFGamePVPModule::OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	if (newVar.GetInt() <= 0)
	{
		const NFGUID& identAttacker = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::LastAttacker());
		const int nSceneID = m_pKernelModule->GetPropertyInt32(identAttacker, NFrame::Player::SceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(identAttacker, NFrame::Player::GroupID());
		const NFGUID matchID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
		const NFMsg::ESceneType sceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(nSceneID), NFrame::Scene::Type());
		const std::string& strCnfID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
		NFMsg::ENPCType eNPCType = (NFMsg::ENPCType)(m_pElementModule->GetPropertyInt(strCnfID, NFrame::NPC::NPCType()));

		if (!identAttacker.IsNull() 
			&& !matchID.IsNull() 
			&& sceneType == NFMsg::ESceneType::SCENE_HOME
			&& eNPCType == NFMsg::ENPCType::ENPCTYPE_HERO)
		{
			if (m_pKernelModule->GetPropertyString(identAttacker, NFrame::IObject::ClassName()) == NFrame::Player::ThisName())
			{
				//is hero?
				const int nExp = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::EXP());
				
				const int nFightingStar = m_pSceneModule->GetPropertyInt32(nSceneID, nGroupID, NFrame::Group::MatchStar());
				const int nkilledHero = m_pSceneModule->GetPropertyInt32(nSceneID, nGroupID, NFrame::Group::MatchOpponentK());
				const int nBekilledHero = m_pSceneModule->GetPropertyInt32(nSceneID, nGroupID, NFrame::Group::MatchOpponentD());
				const NFGUID xViewOpponent = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchOpponentID());
				if (nFightingStar < 3)
				{
					m_pSceneModule->SetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchStar(), nFightingStar + 1);
				}

				m_pSceneModule->SetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchOpponentK(), nkilledHero + 1);
			}
		}
	}

	return 0;
}

int NFGamePVPModule::OnMatchKillsEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

int NFGamePVPModule::OnMatchDeathsEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

int NFGamePVPModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent)
	{
		//off line? disconnect?
		//reconnect?
		const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
		const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
		const NFGUID matchID = m_pSceneModule->GetPropertyObject(sceneID, groupID, NFrame::Group::MatchID());
		const NFMsg::ESceneType scenetype = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(sceneID), NFrame::Scene::Type());
		if (scenetype == NFMsg::ESceneType::SCENE_HOME
			&& !matchID.IsNull())
		{
			//single mode
			EndTheBattle(self, 0);
		}
		else
		{

		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::HP(), this, &NFGamePVPModule::OnPlayerHPEvent);
	}

	return 0;
}

int NFGamePVPModule::OnPlayerHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	if (newVar.GetInt() <= 0)
	{
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
		const NFGUID matchID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
		const NFMsg::ESceneType sceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(nSceneID), NFrame::Scene::Type());
		const std::string& strCnfID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());

		if (!matchID.IsNull()
			&& sceneType == NFMsg::ESceneType::SCENE_HOME)
		{
			const int nBekilledHero = m_pSceneModule->GetPropertyInt32(nSceneID, nGroupID, NFrame::Group::MatchOpponentD());
			m_pSceneModule->SetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchOpponentD(), nBekilledHero + 1);
		}
	}

	return 0;
}

void NFGamePVPModule::EndTheBattle(const NFGUID & self, const int autoEnd)
{
	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType != NFMsg::ESceneType::SCENE_HOME)
	{
		return;
	}


	const NFGUID& matchID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
	if (matchID.IsNull())
	{
		return;
	}


	std::ostringstream stream;
	stream << self.ToString() << " want to end the match, match'id:" << matchID.ToString();

	//depends play mode: single mode or multi mode
	//if single mode, it would end the match if the player disconnect
	//if multi-player mode, it would hold on the status if the player disconnect
	NF_SHARE_PTR<NFIRecord> xMatchMember = m_pSceneModule->FindRecord(nSceneID, nGroupID, NFrame::Group::MatchMember::ThisName());
	if (xMatchMember)
	{
		int64_t nGambleGold = m_pSceneModule->GetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchOpponentGold());
		int64_t nGambleDiamond = m_pSceneModule->GetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchOpponentDiamond());
		int64_t nLevel = m_pSceneModule->GetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchOpponentLevel());
		int64_t nFightingStar = m_pSceneModule->GetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchStar());
		int64_t nWinDiamond = m_pSceneModule->GetPropertyInt(nSceneID, nGroupID, NFrame::Group::MatchGambleDiamond());
		NFGUID matchTeamID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchTeamID());
		NFGUID matchID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());
		
		int64_t nWinCup = 50;
		int64_t nWinGold = 50;
		const int currentCup = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Cup());

		int nExp = nLevel * 100;
		m_pLevelModule->AddExp(self, nExp);

		bool bWin = false;
		if (nFightingStar >= 2)
		{
			bWin = true;
		}

		if (bWin)
		{
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::Cup(), nWinCup + currentCup);
			m_pPropertyModule->AddGold(self, nWinGold);
			m_pPropertyModule->AddDiamond(self, nWinDiamond);
		}
		else
		{
			int cup = currentCup - nWinCup;
			if (cup < 0)
			{
				cup = 0;
			}

			m_pKernelModule->SetPropertyInt(self, NFrame::Player::Cup(), cup);
			m_pPropertyModule->ConsumeGold(self, nWinGold);
			m_pPropertyModule->ConsumeDiamond(self, nWinDiamond);
		}


		NFMsg::AckEndBattle xReqAckEndBattle;
		xReqAckEndBattle.set_battle_mode(NFMsg::EBattleType::EBT_SINGLE_MODE);
		*xReqAckEndBattle.mutable_team_id() = NFINetModule::NFToPB(matchTeamID);
		*xReqAckEndBattle.mutable_match_id() = NFINetModule::NFToPB(matchID);
		NFMsg::Ident* pMember = xReqAckEndBattle.add_members();
		if (pMember)
		{
			*pMember = NFINetModule::NFToPB(self);
		}


		if (bWin)
		{
			xReqAckEndBattle.set_win(1);
			xReqAckEndBattle.set_star(nFightingStar);

			xReqAckEndBattle.set_gold(nWinGold);
			xReqAckEndBattle.set_diamond(nWinDiamond);
			xReqAckEndBattle.set_cup(nWinCup);
			//how to reduce the money for that people who lose the game? maybe that guy onlien maybe not.

			stream << "WIN:" << nWinCup << " STAR:" << nFightingStar;

		}
		else
		{
			//how to reduce the money and diamong if the player not on line?
			//if the player now online, we send a message to the server to change the data
			//if the player not online now, we send the message to the world to change the data from database

			//m_pPropertyModule->AddGold(nPlayerID, -nWinGold);
			//m_pPropertyModule->AddDiamond(nPlayerID, -nWinDiamond);

			xReqAckEndBattle.set_win(0);
			xReqAckEndBattle.set_star(nFightingStar);

			xReqAckEndBattle.set_gold(-nWinGold);
			xReqAckEndBattle.set_diamond(-nWinDiamond);
			xReqAckEndBattle.set_cup(-nWinCup);

			//how to increase the money for that people who won the game? maybe that guy onlien maybe not.
			stream << " LOSE:" << nWinCup << " STAR:" << nFightingStar;
		}


		RecordPVPData(self, nFightingStar, nWinGold, nWinDiamond);

		//ResetPVPData(self);

		//just show the result ui
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_END_OPPNENT, xReqAckEndBattle, self);
		m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_END_OPPNENT, xReqAckEndBattle);
	}
	else
	{

	}
}

/*

void NFGamePVPModule::OnReqAddGambleProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	//attack record and beattack record
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAddGambleValue);

	if (xMsg.number() <= 0)
	{
		return;
	}

	if (xMsg.property() == NFrame::Player::GambleGold())
	{
		if (m_pPropertyModule->ConsumeGold(nPlayerID, xMsg.number()))
		{
			int64_t nGambleGold = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GambleGold());
			m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::GambleGold(), nGambleGold + xMsg.number());
		}
	}
	else if (xMsg.property() == NFrame::Player::GambleDiamond())
	{
		if (m_pPropertyModule->ConsumeDiamond(nPlayerID, xMsg.number()))
		{
			int64_t nGambleDiamond = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GambleDiamond());
			m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::GambleDiamond(), nGambleDiamond + xMsg.number());
		}
	}
}
*/