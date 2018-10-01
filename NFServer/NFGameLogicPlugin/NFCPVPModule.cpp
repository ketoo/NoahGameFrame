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

#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFCPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCPVPModule::Init()
{
	
    return true;
}


bool NFCPVPModule::Shut()
{
    return true;
}

bool NFCPVPModule::Execute()
{
    
    return true;
}

bool NFCPVPModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCPVPModule::OnNPCClassEvent);
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPVPModule::OnPlayerClassEvent);

	m_pSceneAOIModule->AddEnterSceneConditionCallBack(this, &NFCPVPModule::EnterSceneConditionEvent);

	m_pSceneAOIModule->AddBeforeEnterSceneGroupCallBack(this, &NFCPVPModule::BeforeEnterSceneGroupEvent);
	m_pSceneAOIModule->AddAfterEnterSceneGroupCallBack(this, &NFCPVPModule::AfterEnterSceneGroupEvent);
	m_pSceneAOIModule->AddBeforeLeaveSceneGroupCallBack(this, &NFCPVPModule::BeforeLeaveSceneGroupEvent);
	m_pSceneAOIModule->AddAfterLeaveSceneGroupCallBack(this, &NFCPVPModule::AfterLeaveSceneGroupEvent);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFCPVPModule::OnReqSearchOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_HOME_SCENE, this, &NFCPVPModule::OnReqSwapHomeSceneProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_START_OPPNENT, this, &NFCPVPModule::OnReqStartPVPOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_END_OPPNENT, this, &NFCPVPModule::OnReqEndPVPOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ADJUST_GAMBLE, this, &NFCPVPModule::OnReqAddGambleProcess)) { return false; }


	if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_SEARCH_OPPNENT, this, &NFCPVPModule::OnAckSearchOpponentProcess)) { return false; }

    return true;
}

bool NFCPVPModule::ReadyExecute()
{
	m_pSceneAOIModule->RemoveSwapSceneEventCallBack();
	m_pSceneAOIModule->AddSwapSceneEventCallBack(this, &NFCPVPModule::OnSceneEvent);

	return false;
}

void NFCPVPModule::OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                              const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);
	int pvpType = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::PVPType());
	if (pvpType != NFMsg::EPVPType::PVP_HOME)
	{
		//return;
	}

	ResetPVPData(nPlayerID);

	int nHomeSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::HomeSceneID());
	xMsg.set_self_scene(nHomeSceneID);
	int nHasKey = nPlayerID.nData64;
	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, nMsgID, xMsg, nPlayerID);
}

void NFCPVPModule::OnAckSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::AckSearchOppnent);

	if (!m_pKernelModule->ExistObject(nPlayerID))
	{
		return;
	}

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent(), NFINetModule::PBToNF(xMsg.opponent()));
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightingOpponent(), NFGUID());
	m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::FightingStar(), 0);

	m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::PVPType(), NFMsg::EPVPType::PVP_INDIVIDUAL);

	m_pSceneProcessModule->RequestEnterScene(nPlayerID, xMsg.scene_id(), 0, NFDataList());

	int nNoWSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
	int nGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());

	if (nNoWSceneID != xMsg.scene_id())
	{
		return;
	}
	
	//process opponent data
	ProcessOpponentData(nPlayerID, xMsg);

	m_pGameServerNet_ServerModule->SendMsgToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, std::string(msg, nLen), nPlayerID);

	for (int i = 0; i < xMsg.title().building_size(); ++i)
	{
		const NFMsg::TileBuilding& xTileBuilding = xMsg.title().building().Get(i);

		NFGUID xBuildingID = NFINetModule::PBToNF(xTileBuilding.guid());
		NFVector3 vPos;
		vPos.SetX(xTileBuilding.x());
		vPos.SetY(xTileBuilding.y());
		std::string strCnfID = xTileBuilding.configid();

		NFDataList xDataArg;
		xDataArg.AddString(NFrame::NPC::Position());
		xDataArg.AddVector3(vPos);
		xDataArg.AddString(NFrame::NPC::MasterID());
		xDataArg.AddObject(NFINetModule::PBToNF(xMsg.opponent()));
		xDataArg.AddString(NFrame::NPC::AIOwnerID());
		xDataArg.AddObject(nPlayerID);
		xDataArg.AddString(NFrame::NPC::NPCType());
		xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

		m_pKernelModule->CreateObject(xBuildingID, nNoWSceneID, nGroupID, NFrame::NPC::ThisName(), strCnfID, xDataArg);
	}
}

void NFCPVPModule::OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckHomeScene);
	int nHomeSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::HomeSceneID());
	int nSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::SceneID());
	int pvpType = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::PVPType());
	
	if (pvpType == NFMsg::EPVPType::PVP_HOME)
	{
		return;
	}

    ResetPVPData(nPlayerID);

    m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::PVPType(), NFMsg::EPVPType::PVP_HOME);

	m_pSceneProcessModule->RequestEnterScene(nPlayerID, nHomeSceneID, 0, NFDataList());

	
}

void NFCPVPModule::OnReqStartPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
												const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckStartBattle);
	int pvpType = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::PVPType());
	if (pvpType == NFMsg::EPVPType::PVP_HOME)
	{
		return;
	}

	NFGUID xWarGUID = m_pKernelModule->CreateGUID();
	NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent());

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightingOpponent(), xViewOpponent);
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::WarID(), xWarGUID);
	m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::WarEventTime(), NFGetTimeMS());

	int nGambleGold = xMsg.gold();
	int nGambleDiamond = xMsg.diamond();
	if (nGambleGold > 0)
	{
		m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::GambleGold(), nGambleGold);
	}
	if (nGambleDiamond > 0)
	{
		m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::GambleDiamond(), nGambleDiamond);
	}

	NFMsg::ReqAckStartBattle xReqAckStartBattle;
	xReqAckStartBattle.set_gold(nGambleGold);
	xReqAckStartBattle.set_diamond(nGambleDiamond);
	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_START_OPPNENT, xReqAckStartBattle, nPlayerID);
}

void NFCPVPModule::OnReqEndPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
											  const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqEndBattle);

	//get oppnent
	//calculate how many monster has been killed
	//calculate how many building has been destroyed
	//calculate how much experice and how many money

	//tell client the end information
	//set oppnent 0
	int pvpType = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::PVPType());
	if (pvpType == NFMsg::EPVPType::PVP_INDIVIDUAL)
	{
		EndTheBattle(nPlayerID);
	}

}

int NFCPVPModule::OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::string strTileData;
	NFVector3 vRelivePos = m_pSceneAOIModule->GetRelivePosition(nSceneID, 0);
	NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(self, NFrame::Player::ViewOpponent());
	NFMsg::EPVPType ePvpType = (NFMsg::EPVPType)m_pKernelModule->GetPropertyInt(self, NFrame::Player::PVPType());

	NFMsg::ReqAckSwapScene xAckSwapScene;
	xAckSwapScene.set_scene_id(nSceneID);
	xAckSwapScene.set_transfer_type(NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL);
	xAckSwapScene.set_line_id(0);
	xAckSwapScene.set_x(vRelivePos.X());
	xAckSwapScene.set_y(vRelivePos.Y());
	xAckSwapScene.set_z(vRelivePos.Z());

	if (ePvpType == NFMsg::EPVPType::PVP_HOME)
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOpponent(), NFGUID());

		//if (m_pTileModule->GetOnlinePlayerTileData(self, strTileData))
		{
			xAckSwapScene.set_data(strTileData.c_str(), strTileData.length());
		}
	}
	else if(ePvpType == NFMsg::EPVPType::PVP_INDIVIDUAL)
	{
		//if (m_pPlayerRedisModule->LoadPlayerTileRandomCache(xViewOpponent, strTileData))
		{
			xAckSwapScene.set_data(strTileData.c_str(), strTileData.length());
		}
	}
	

	//attach tile data
	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_SCENE, xAckSwapScene, self);

	return 0;
}

int NFCPVPModule::EnterSceneConditionEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCPVPModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCPVPModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//create building if the player back home
	//create building if the player wants attack the others

	ResetPVPData(self);

	return 0;
}

int NFCPVPModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCPVPModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCPVPModule::OnDeadSwapHeroHeart(const NFGUID & self, const std::string & strHeartBeat, const float fTime, const int nCount)
{
	NFGUID xFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	NFGUID xHero1 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1());
	NFGUID xHero2 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2());
	NFGUID xHero3 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3());

	int nHeroHP1 = m_pKernelModule->GetPropertyInt(self, NFrame::Player::FightHeroHP1());
	int nHeroHP2 = m_pKernelModule->GetPropertyInt(self, NFrame::Player::FightHeroHP2());
	int nHeroHP3 = m_pKernelModule->GetPropertyInt(self, NFrame::Player::FightHeroHP3());

	if (nHeroHP1 <= 0 && nHeroHP2 <= 0 && nHeroHP3 <= 0)
	{
		//all dead, end the battle
		EndTheBattle(self);

		return 0;
	}

	if (nHeroHP1 > 0)
	{
		m_pHeroModule->SwitchFightHero(self, xHero1);
	}
	else if (nHeroHP2 > 0)
	{
		m_pHeroModule->SwitchFightHero(self, xHero2);
	}
	else if (nHeroHP3 > 0)
	{
		m_pHeroModule->SwitchFightHero(self, xHero3);
	}
	//swap hero

	return 0;
}

bool NFCPVPModule::ProcessOpponentData(const NFGUID & self, const NFMsg::AckSearchOppnent& opponent)
{

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

	//tell client u shoud adjust tile
	//m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, nPlayerID);
	//tell client u should load resources



	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentGold(), opponent.gold());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentDiamond(), opponent.diamond());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentLevel(), opponent.level());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentCup(), opponent.cup());
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentName(), opponent.name());
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHead(), opponent.head());

	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero1(), opponent.hero_cnf1());
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero2(), opponent.hero_cnf2());
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero3(), opponent.hero_cnf3());

	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar1(), opponent.hero_star1());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar2(), opponent.hero_star2());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar3(), opponent.hero_star3());

	NFVector3 xHeroBornPos1 = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::HeroPos1());
	NFVector3 xHeroBornPos2 = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::HeroPos2());
	NFVector3 xHeroBornPos3 = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::HeroPos3());
	
	//create building and hero
	NFDataList xHeroData1;
	xHeroData1.AddString(NFrame::NPC::Position());
	xHeroData1.AddVector3(NFINetModule::PBToNF(opponent.hero_pos1()));
	xHeroData1.AddString(NFrame::NPC::MasterID());
	xHeroData1.AddObject(NFINetModule::PBToNF(opponent.opponent()));
	xHeroData1.AddString(NFrame::NPC::AIOwnerID());
	xHeroData1.AddObject(self);
	xHeroData1.AddString(NFrame::NPC::NPCType());
	xHeroData1.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
	xHeroData1.AddString(NFrame::NPC::Level());
	xHeroData1.AddInt(opponent.level());
	xHeroData1.AddString(NFrame::NPC::HeroStar());
	xHeroData1.AddInt(opponent.hero_star1());

	NFDataList xHeroData2;

	xHeroData2.AddString(NFrame::NPC::Position());
	xHeroData2.AddVector3((NFINetModule::PBToNF(opponent.hero_pos2())));
	xHeroData2.AddString(NFrame::NPC::MasterID());
	xHeroData2.AddObject(NFINetModule::PBToNF(opponent.opponent()));
	xHeroData2.AddString(NFrame::NPC::AIOwnerID());
	xHeroData2.AddObject(self);
	xHeroData2.AddString(NFrame::NPC::NPCType());
	xHeroData2.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
	xHeroData2.AddString(NFrame::NPC::Level());
	xHeroData2.AddInt(opponent.level());
	xHeroData2.AddString(NFrame::NPC::HeroStar());
	xHeroData2.AddInt(opponent.hero_star2());

	NFDataList xHeroData3;
	xHeroData3.AddString(NFrame::NPC::Position());
	xHeroData3.AddVector3((NFINetModule::PBToNF(opponent.hero_pos3())));
	xHeroData3.AddString(NFrame::NPC::MasterID());
	xHeroData3.AddObject(NFINetModule::PBToNF(opponent.opponent()));
	xHeroData3.AddString(NFrame::NPC::AIOwnerID());
	xHeroData3.AddObject(self);
	xHeroData3.AddString(NFrame::NPC::NPCType());
	xHeroData3.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
	xHeroData3.AddString(NFrame::NPC::Level());
	xHeroData3.AddInt(opponent.level());
	xHeroData3.AddString(NFrame::NPC::HeroStar());
	xHeroData3.AddInt(opponent.hero_star3());

	m_pKernelModule->CreateObject(NFINetModule::PBToNF(opponent.hero_id1()), nSceneID, nGroupID, NFrame::NPC::ThisName(), opponent.hero_cnf1(), xHeroData1);
	m_pKernelModule->CreateObject(NFINetModule::PBToNF(opponent.hero_id2()), nSceneID, nGroupID, NFrame::NPC::ThisName(), opponent.hero_cnf2(), xHeroData2);
	m_pKernelModule->CreateObject(NFINetModule::PBToNF(opponent.hero_id3()), nSceneID, nGroupID, NFrame::NPC::ThisName(), opponent.hero_cnf3(), xHeroData3);

	return true;
}

void NFCPVPModule::ResetPVPData(const NFGUID & self)
{
	m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOpponent(), NFGUID());
	m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightingOpponent(), NFGUID());

	m_pKernelModule->SetPropertyObject(self, NFrame::Player::WarID(), NFGUID());
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::WarEventTime(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::FightingStar(),0);

	m_pKernelModule->SetPropertyInt(self, NFrame::Player::WonGold(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::WonDiamond(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::WonCup(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentGold(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentDiamond(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentLevel(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentCup(), 0);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentName(), "");
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHead(), "");

	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero1(), "");
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar1(), 0);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero2(), "");
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar2(), 0);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero3(), "");
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHeroStar3(), 0);

	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item1UsedCount(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item2UsedCount(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item3UsedCount(), 0);

	m_pKernelModule->ClearRecord(self, NFrame::Player::TempItemList::ThisName());
}

void NFCPVPModule::RecordPVPData(const NFGUID & self, const int nStar, const int nGold, const int nDiamond)
{
	//how to record this war for these two people
	NFGUID xWarID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::WarID());

	NF_SHARE_PTR<NFIRecord> xAttackWarRecord = m_pKernelModule->FindRecord(self, NFrame::Player::WarList::ThisName());

	NF_SHARE_PTR<NFDataList> xDataList = xAttackWarRecord->GetInitData();
	xDataList->SetObject(NFrame::Player::WarList::WarID, xWarID);
	//record the war information
	/*
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
	*/

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
	xDataList->SetObject(NFrame::Player::WarList::BeAttackerID, m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightingOpponent()));
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
		xDataList->SetObject(NFrame::Player::WarList::Winner, m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightingOpponent()));
	}

	xAttackWarRecord->AddRow(-1, *xDataList);

	////////for beattacker////////////////////////////////////



}

int NFCPVPModule::OnNPCClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFCPVPModule::OnNPCHPEvent);
	}

	return 0;
}

int NFCPVPModule::OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	if (newVar.GetInt() <= 0)
	{
		const NFGUID& identAttacker = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::LastAttacker());
		const std::string& strCnfID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
		int nExp = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::EXP());

		if (!identAttacker.IsNull())
		{
			if (m_pKernelModule->GetPropertyString(identAttacker, NFrame::IObject::ClassName()) == NFrame::Player::ThisName())
			{
				//is hero?
				int nHomeSceneID = m_pKernelModule->GetPropertyInt32(identAttacker, NFrame::Player::HomeSceneID());
				int nSceneID = m_pKernelModule->GetPropertyInt32(identAttacker, NFrame::Player::SceneID());
				int nFightingStar = m_pKernelModule->GetPropertyInt32(identAttacker, NFrame::Player::FightingStar());
				int nCup = m_pKernelModule->GetPropertyInt(identAttacker, NFrame::Player::Cup());
				int nOpponentCup = m_pKernelModule->GetPropertyInt(identAttacker, NFrame::Player::OpponentCup());
				int pvpType = m_pKernelModule->GetPropertyInt(identAttacker, NFrame::Player::PVPType());

				NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(identAttacker, NFrame::Player::ViewOpponent());
				NFGUID xFightingOpponent = m_pKernelModule->GetPropertyObject(identAttacker, NFrame::Player::FightingOpponent());
			
				if (pvpType == NFMsg::EPVPType::PVP_INDIVIDUAL)
				{
					NFMsg::ENPCType eNPCType = (NFMsg::ENPCType)(m_pElementModule->GetPropertyInt(strCnfID, NFrame::NPC::NPCType()));
					if (eNPCType == NFMsg::ENPCType::ENPCTYPE_HERO)
					{
						nFightingStar++;
						m_pKernelModule->SetPropertyInt(identAttacker, NFrame::Player::FightingStar(), nFightingStar);

						if (nFightingStar >= 2)
						{
							if (nCup > nOpponentCup)
							{
								m_pKernelModule->SetPropertyInt(identAttacker, NFrame::Player::WonCup(), 5 * nFightingStar);
							}
							else
							{
								m_pKernelModule->SetPropertyInt(identAttacker, NFrame::Player::WonCup(), 3 * nFightingStar);
							}
						}
					}

					//add exp for hero
				}
			}
		}
	}

	return 0;
}

int NFCPVPModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::FightHeroHP1(), this, &NFCPVPModule::OnPlayerHPEvent);
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::FightHeroHP2(), this, &NFCPVPModule::OnPlayerHPEvent);
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::FightHeroHP3(), this, &NFCPVPModule::OnPlayerHPEvent);
	}

	return 0;
}

int NFCPVPModule::OnPlayerHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	//is pvp
	int pvpType = m_pKernelModule->GetPropertyInt(self, NFrame::Player::PVPType());
	if (pvpType == NFMsg::EPVPType::PVP_INDIVIDUAL)
	{
		if (newVar.GetInt() <= 0)
		{
			m_pScheduleModule->AddSchedule(self, "NFCPVPModule::OnDeadSwapHeroHeart", this, &NFCPVPModule::OnDeadSwapHeroHeart, 5.0f, 1);
		}
	}

	return 0;
}

void NFCPVPModule::EndTheBattle(const NFGUID & self)
{
	int64_t nGambleGold = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentGold());
	int64_t nGambleDiamond = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentDiamond());
	int64_t nLevel = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentLevel());
	int64_t nFightingStar = m_pKernelModule->GetPropertyInt(self, NFrame::Player::FightingStar());
	int64_t nWonGold = m_pKernelModule->GetPropertyInt(self, NFrame::Player::WonGold());
	int64_t nWonDiamond = m_pKernelModule->GetPropertyInt(self, NFrame::Player::WonDiamond());
	int64_t nWonCup = m_pKernelModule->GetPropertyInt(self, NFrame::Player::WonCup());
	int64_t nCup = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Cup());


	bool bWon = false;
	if (nFightingStar >= 2)
	{
		bWon = true;
	}

	if (bWon)
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Cup(), nWonCup + nCup);
	}

	int nExp = nLevel * 100;
	m_pPropertyModule->AddGold(self, nWonGold);
	m_pPropertyModule->AddDiamond(self, nWonDiamond);
	m_pLevelModule->AddExp(self, nExp);

	NFMsg::AckEndBattle xReqAckEndBattle;
	if (bWon)
	{
		xReqAckEndBattle.set_win(1);
		xReqAckEndBattle.set_star(nFightingStar);
		xReqAckEndBattle.set_exp(nExp);

		xReqAckEndBattle.set_gold(nWonGold);
		xReqAckEndBattle.set_diamond(nWonDiamond);

		//how to reduce the money for that people who lose the game? maybe that guy onlien maybe not.
	}
	else
	{
		//m_pPropertyModule->AddGold(nPlayerID, -nWinGold);
		//m_pPropertyModule->AddDiamond(nPlayerID, -nWinDiamond);

		xReqAckEndBattle.set_win(0);
		xReqAckEndBattle.set_star(nFightingStar);
		xReqAckEndBattle.set_exp(nExp);

		xReqAckEndBattle.set_gold(-nWonGold);
		xReqAckEndBattle.set_diamond(-nWonDiamond);

		//how to increase the money for that people who won the game? maybe that guy onlien maybe not.
	}


	RecordPVPData(self, nFightingStar, nWonGold, nWonDiamond);

	//just show the result ui
	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_END_OPPNENT, xReqAckEndBattle, self);
}

void NFCPVPModule::OnReqAddGambleProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
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
