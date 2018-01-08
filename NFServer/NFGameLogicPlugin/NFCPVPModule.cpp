// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFCPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCPVPModule::Init()
{
	
	m_pTileModule = pPluginManager->FindModule<NFITileModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
	

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
	FindAllTileScene();

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCPVPModule::OnNPCClassEvent);

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

	ResetPVPData(nPlayerID);

	int nSearchCount = 0;
	bool bRet = false;
	while (!bRet && nSearchCount <= 5)
	{
		bRet = SearchOpponent(nPlayerID);
		nSearchCount++;
	}

	if (!bRet)
	{
		//failed
		NFMsg::AckSearchOppnent xAckData;
		xAckData.set_scene_id(0);
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nPlayerID);

		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "ERROR TO FIND A OPPNENT!", "", __FUNCTION__, __LINE__);
	}
}

void NFCPVPModule::OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckHomeScene);
	int nHomeSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::HomeSceneID());
	int nSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::SceneID());

	NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent());
	NFGUID xFightingOpponent = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::FightingOpponent());

	if (nHomeSceneID == nSceneID
		&& xViewOpponent.IsNull()
		&& xFightingOpponent.IsNull())
	{
		return;
	}

    ResetPVPData(nPlayerID);

    m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::PVPType(), NFIPVPModule::PVP_HOME);

	m_pSceneProcessModule->RequestEnterScene(nPlayerID, nHomeSceneID, 0, NFDataList());
}

void NFCPVPModule::OnReqStartPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
												const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckStartBattle);

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
	int64_t nGambleGold = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::OpponentGold());
	int64_t nGambleDiamond = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::OpponentDiamond());
	int64_t nLevel = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::OpponentLevel());
	int64_t nFightingStar = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::FightingStar());
	int64_t nWonGold = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::WonGold());
	int64_t nWonDiamond = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::WonDiamond());
	int64_t nWonCup = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::WonCup());
	int64_t nCup = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::Cup());
	

	bool bWon = false;
	if (nFightingStar >= 2)
	{
		bWon = true;
	}
	
	if(bWon)
	{
		m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::Player::Cup(), nWonCup + nCup);
	}

	int nExp = nLevel * 100;
	m_pPropertyModule->AddGold(nPlayerID, nWonGold);
	m_pPropertyModule->AddDiamond(nPlayerID, nWonDiamond);
	m_pLevelModule->AddExp(nPlayerID, nExp);

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

	
	RecordPVPData(nPlayerID, nFightingStar, nWonGold, nWonDiamond);


	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_END_OPPNENT, xReqAckEndBattle, nPlayerID);

}

void NFCPVPModule::FindAllTileScene()
{
	//Tile
	//mxTileSceneIDList
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Scene::Tile());
			if (nServerType == 1)
			{
				mxTileSceneIDList.push_back(lexical_cast<int>(strId));
			}
		}
	}
}

void NFCPVPModule::InitAllTileSceneRobot()
{
	for (int i = 0; i < mxTileSceneIDList.size(); ++i)
	{
		int nSceneID = mxTileSceneIDList[i];

	}
}

int NFCPVPModule::OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::string strTileData;
	NFVector3 vRelivePos = m_pSceneAOIModule->GetRelivePosition(nSceneID, 0);
	NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(self, NFrame::Player::ViewOpponent());

	NFMsg::ReqAckSwapScene xAckSwapScene;
	xAckSwapScene.set_scene_id(nSceneID);
	xAckSwapScene.set_transfer_type(NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL);
	xAckSwapScene.set_line_id(0);
	xAckSwapScene.set_x(vRelivePos.X());
	xAckSwapScene.set_y(vRelivePos.Y());
	xAckSwapScene.set_z(vRelivePos.Z());

	if (xViewOpponent.IsNull() || self == xViewOpponent)
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOpponent(), NFGUID());

		if (m_pTileModule->GetOnlinePlayerTileData(self, strTileData))
		{
			xAckSwapScene.set_data(strTileData.c_str(), strTileData.length());
		}
	}
	else
	{
		if (m_pPlayerRedisModule->LoadPlayerTileRandomCache(xViewOpponent, strTileData))
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

bool NFCPVPModule::ProcessOpponentData(const NFGUID & self, const NFGUID& opponent)
{

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

	//tell client u shoud adjust tile
	//m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, nPlayerID);
	//tell client u should load resources
	//gold, diamong, awards, level, name, headicon, 3 heros and the star of that heros
	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;

	vKeyList.push_back(NFrame::Player::Gold());
	vKeyList.push_back(NFrame::Player::Diamond());
	vKeyList.push_back(NFrame::Player::Level());
	vKeyList.push_back(NFrame::Player::Cup());
	vKeyList.push_back(NFrame::Player::Name());
	vKeyList.push_back(NFrame::Player::Head());
	vKeyList.push_back(NFrame::Player::HeroPos1CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos1Star());
	vKeyList.push_back(NFrame::Player::HeroPos2CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos2Star());
	vKeyList.push_back(NFrame::Player::HeroPos3CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos3Star());

	//12-14
	vKeyList.push_back(NFrame::Player::HeroID1());
	vKeyList.push_back(NFrame::Player::HeroID2());
	vKeyList.push_back(NFrame::Player::HeroID3());

	//try
	const std::string& strPlayerKey = m_pCommonRedisModule->GetPropertyCacheKey(opponent);
	if (m_pNoSqlModule->HMGet(strPlayerKey, vKeyList, vValueList))
	{
		int nGold = lexical_cast<int>(vValueList.at(0));
		int nDiamond = lexical_cast<int>(vValueList.at(1));
		int nLevel = lexical_cast<int>(vValueList.at(2));
		int nCup = lexical_cast<int>(vValueList.at(3));
		std::string strName = vValueList.at(4);
		std::string strHead = vValueList.at(5);

		std::string strHero1CnfID = vValueList.at(6);
		int nHero1Star = lexical_cast<int>(vValueList.at(7));
		std::string strHero2CnfID = vValueList.at(8);
		int nHero2Star = lexical_cast<int>(vValueList.at(9));
		std::string strHero3CnfID = vValueList.at(10);
		int nHero3Star = lexical_cast<int>(vValueList.at(11));

		NFGUID xHero1;
		NFGUID xHero2;
		NFGUID xHero3;

		xHero1.FromString(vValueList.at(12));
		xHero2.FromString(vValueList.at(13));
		xHero3.FromString(vValueList.at(14));

		NFVector3 xHeroPos1 = m_pSceneAOIModule->GetTagPosition(nSceneID, 0);
		NFVector3 xHeroPos2 = m_pSceneAOIModule->GetTagPosition(nSceneID, 1);
		NFVector3 xHeroPos3 = m_pSceneAOIModule->GetTagPosition(nSceneID, 2);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentGold(), nGold);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentDiamond(), nDiamond);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentLevel(), nLevel);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentCup(), nCup);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentName(), strName);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHead(), strHead);

		m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero1(), strHero1CnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero1Star(), nHero1Star);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero2(), strHero2CnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero2Star(), nHero2Star);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero3(), strHero3CnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero3Star(), nHero3Star);

		//create building and hero
		NFDataList xHeroData1;
		xHeroData1.AddString(NFrame::NPC::Position());
		xHeroData1.AddVector3(xHeroPos1);
		xHeroData1.AddString(NFrame::NPC::MasterID());
		xHeroData1.AddObject(opponent);
		xHeroData1.AddString(NFrame::NPC::AIOwnerID());
		xHeroData1.AddObject(self);
		xHeroData1.AddString(NFrame::NPC::NPCType());
		xHeroData1.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
		xHeroData1.AddString(NFrame::NPC::Level());
		xHeroData1.AddInt(nLevel);
		xHeroData1.AddString(NFrame::NPC::HeroStar());
		xHeroData1.AddInt(nHero1Star);

		NFDataList xHeroData2;
		
		xHeroData2.AddString(NFrame::NPC::Position());
		xHeroData2.AddVector3(xHeroPos2);
		xHeroData2.AddString(NFrame::NPC::MasterID());
		xHeroData2.AddObject(opponent);
		xHeroData2.AddString(NFrame::NPC::AIOwnerID());
		xHeroData2.AddObject(self);
		xHeroData2.AddString(NFrame::NPC::NPCType());
		xHeroData2.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
		xHeroData2.AddString(NFrame::NPC::Level());
		xHeroData2.AddInt(nLevel);
		xHeroData2.AddString(NFrame::NPC::HeroStar());
		xHeroData2.AddInt(nHero2Star);

		NFDataList xHeroData3;
		xHeroData3.AddString(NFrame::NPC::Position());
		xHeroData3.AddVector3(xHeroPos3);
		xHeroData3.AddString(NFrame::NPC::MasterID());
		xHeroData3.AddObject(opponent);
		xHeroData3.AddString(NFrame::NPC::AIOwnerID());
		xHeroData3.AddObject(self);
		xHeroData3.AddString(NFrame::NPC::NPCType());
		xHeroData3.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);
		xHeroData3.AddString(NFrame::NPC::Level());
		xHeroData3.AddInt(nLevel);
		xHeroData3.AddString(NFrame::NPC::HeroStar());
		xHeroData3.AddInt(nHero3Star);

		m_pKernelModule->CreateObject(xHero1, nSceneID, nGroupID, NFrame::NPC::ThisName(), strHero1CnfID, xHeroData1);
		m_pKernelModule->CreateObject(xHero2, nSceneID, nGroupID, NFrame::NPC::ThisName(), strHero2CnfID, xHeroData2);
		m_pKernelModule->CreateObject(xHero3, nSceneID, nGroupID, NFrame::NPC::ThisName(), strHero3CnfID, xHeroData3);
		
		return true;
	}

	return false;
}

bool NFCPVPModule::SearchOpponent(const NFGUID & self)
{
	int nSceneID = RandomTileScene();
	std::string strTileData;
	NFGUID xViewOpponent;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, xViewOpponent, strTileData) && !xViewOpponent.IsNull())
	{
		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOpponent(), xViewOpponent);
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightingOpponent(), NFGUID());
			m_pKernelModule->SetPropertyInt(self, NFrame::Player::FightingStar(), 0);

			m_pKernelModule->SetPropertyInt(self, NFrame::Player::PVPType(), NFIPVPModule::PVP_INDIVIDUAL);

			m_pSceneProcessModule->RequestEnterScene(self, nSceneID, 0, NFDataList());
			
			int nNoWSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
			int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

			if (nNoWSceneID != nSceneID)
			{
				//???
			}

			//create the building && remove tile
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, self);

			for (int i = 0; i < xTileData.building_size(); ++i)
			{
				const NFMsg::TileBuilding& xTileBuilding = xTileData.building().Get(i);

				NFGUID xBuildingID = NFINetModule::PBToNF(xTileBuilding.guid());
				NFVector3 vPos;
				vPos.SetX(xTileBuilding.x());
				vPos.SetY(xTileBuilding.y());
				std::string strCnfID = xTileBuilding.configid();

				NFDataList xDataArg;
				xDataArg.AddString(NFrame::NPC::Position());
				xDataArg.AddVector3(vPos);
				xDataArg.AddString(NFrame::NPC::MasterID());
				xDataArg.AddObject(xViewOpponent);
				xDataArg.AddString(NFrame::NPC::AIOwnerID());
				xDataArg.AddObject(self);
				xDataArg.AddString(NFrame::NPC::NPCType());
				xDataArg.AddInt(NFMsg::ENPCType::ENPCTYPE_TURRET);

				m_pKernelModule->CreateObject(xBuildingID, nNoWSceneID, nGroupID, NFrame::NPC::ThisName(), strCnfID, xDataArg);
			}
			

			//process opponent data
			ProcessOpponentData(self, xViewOpponent);

			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(nSceneID);
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, self);

			return true;
		}
		else
		{

			return false;
		}
	}

	return false;
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
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero1Star(), 0);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero2(), "");
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero2Star(), 0);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::OpponentHero3(), "");
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::OpponentHero3Star(), 0);

	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item1UsedCount(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item2UsedCount(), 0);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Item3UsedCount(), 0);

	m_pKernelModule->ClearRecord(self, NFrame::Player::TempItemList::ThisName());
}

void NFCPVPModule::RecordPVPData(const NFGUID & self, const int nStar, const int nGold, const int nDiamond)
{
	//how to record this war for these two people
	NFGUID xWarID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::WarID());

	NF_SHARE_PTR<NFIRecord> xAttackWarRecord = m_pKernelModule->FindRecord(self, NFrame::Player::AttackList::ThisName());

	NF_SHARE_PTR<NFDataList> xDataList = xAttackWarRecord->GetInitData();
	xDataList->SetObject(NFrame::Player::AttackList::WarID, xWarID);
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

	xDataList->SetString(NFrame::Player::AttackList::AttackerName, m_pKernelModule->GetPropertyString(self, NFrame::Player::Name()));
	xDataList->SetInt(NFrame::Player::AttackList::AttackerCup, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Cup()));
	xDataList->SetString(NFrame::Player::AttackList::AttackerHero1, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos1CnfID()));
	xDataList->SetInt(NFrame::Player::AttackList::AttackerHero1Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos1Star()));
	xDataList->SetString(NFrame::Player::AttackList::AttackerHero2, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos2CnfID()));
	xDataList->SetInt(NFrame::Player::AttackList::AttackerHero2Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos2Star()));
	xDataList->SetString(NFrame::Player::AttackList::AttackerHero3, m_pKernelModule->GetPropertyString(self, NFrame::Player::HeroPos3CnfID()));
	xDataList->SetInt(NFrame::Player::AttackList::AttackerHero3Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::HeroPos3Star()));
	xDataList->SetObject(NFrame::Player::AttackList::AttackerID, self);
	xDataList->SetInt(NFrame::Player::AttackList::AttackerLevel, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()));

	xDataList->SetString(NFrame::Player::AttackList::BeAttackerName, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentName()));
	xDataList->SetInt(NFrame::Player::AttackList::BeAttackerCup, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentCup()));
	xDataList->SetString(NFrame::Player::AttackList::BeAttackerHero1, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero1()));
	xDataList->SetInt(NFrame::Player::AttackList::BeAttackerHero1Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero1Star()));
	xDataList->SetString(NFrame::Player::AttackList::BeAttackerHero2, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero2()));
	xDataList->SetInt(NFrame::Player::AttackList::BeAttackerHero2Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero2Star()));
	xDataList->SetString(NFrame::Player::AttackList::BeAttackerHero3, m_pKernelModule->GetPropertyString(self, NFrame::Player::OpponentHero3()));
	xDataList->SetInt(NFrame::Player::AttackList::BeAttackerHero3Star, m_pKernelModule->GetPropertyInt(self, NFrame::Player::OpponentHero3Star()));
	xDataList->SetObject(NFrame::Player::AttackList::BeAttackerID, m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightingOpponent()));
	xDataList->SetInt(NFrame::Player::AttackList::BeAttackerLevel, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()));

	xDataList->SetString(NFrame::Player::AttackList::Item1, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item1()));
	xDataList->SetInt(NFrame::Player::AttackList::Item1Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item1UsedCount()));
	xDataList->SetString(NFrame::Player::AttackList::Item2, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item2()));
	xDataList->SetInt(NFrame::Player::AttackList::Item2Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item2UsedCount()));
	xDataList->SetString(NFrame::Player::AttackList::Item3, m_pKernelModule->GetPropertyString(self, NFrame::Player::Item3()));
	xDataList->SetInt(NFrame::Player::AttackList::Item3Count, m_pKernelModule->GetPropertyInt(self, NFrame::Player::Item3UsedCount()));


	int64_t nEventTime = m_pKernelModule->GetPropertyInt(self, NFrame::Player::WarEventTime());
	int64_t nCostTime = NFGetTimeMS() - nEventTime;

	xDataList->SetInt(NFrame::Player::AttackList::EventTime, nEventTime);
	xDataList->SetInt(NFrame::Player::AttackList::WarStar, nStar);
	xDataList->SetInt(NFrame::Player::AttackList::CostTime, nCostTime);
	xDataList->SetInt(NFrame::Player::AttackList::Gold, nGold);
	xDataList->SetInt(NFrame::Player::AttackList::Diamond, nDiamond);

	if (nStar >= 2)
	{
		xDataList->SetObject(NFrame::Player::AttackList::Winner, self);
	}
	else
	{
		xDataList->SetObject(NFrame::Player::AttackList::Winner, m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightingOpponent()));
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
				NFIPVPModule::PVP_TYPE pvpType = (NFIPVPModule::PVP_TYPE)m_pKernelModule->GetPropertyInt(identAttacker, NFrame::Player::PVPType());

				NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(identAttacker, NFrame::Player::ViewOpponent());
				NFGUID xFightingOpponent = m_pKernelModule->GetPropertyObject(identAttacker, NFrame::Player::FightingOpponent());
			
				if (pvpType == NFIPVPModule::PVP_TYPE::PVP_INDIVIDUAL)
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

int NFCPVPModule::RandomTileScene()
{
	return mxTileSceneIDList.at(m_pKernelModule->Random(0, (int)mxTileSceneIDList.size()));
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
