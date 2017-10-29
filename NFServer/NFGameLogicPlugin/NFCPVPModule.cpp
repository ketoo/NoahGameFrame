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

	int nSearchCount = 0;
	bool bRet = false;
	while (!bRet && nSearchCount <= 5)
	{
		YieldCo(1000 * 2);

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

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent(), nPlayerID);
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOpponent(), NFGUID());

	m_pSceneProcessModule->RequestEnterScene(nPlayerID, nHomeSceneID, 0, NFDataList());

	//create the building of the oppnent

	/*
	//send tile's data to client
	std::string strTileData;
	if (m_pPlayerRedisModule->LoadPlayerTile(nHomeSceneID, nPlayerID, strTileData))
	{
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, strTileData, nPlayerID);
	}
	*/
}

void NFCPVPModule::OnReqStartPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
												const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckStartBattle);

	NFGUID xViewOpponent = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent());
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOpponent(), xViewOpponent);

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

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOpponent(), NFGUID());
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOpponent(), NFGUID());
	//get oppnent
	//calculate how many monster has been killed
	//calculate how many building has been destroyed
	//calculate how much experice and how many money

	//tell client the end information
	//set oppnent 0
	int64_t nGambleGold = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GambleGold());
	int nGambleDiamond = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::GambleDiamond());
	
	m_pPropertyModule->AddGold(nPlayerID, nGambleGold);
	m_pPropertyModule->AddDiamond(nPlayerID, nGambleDiamond);

	NFMsg::AckEndBattle xReqAckEndBattle;
	xReqAckEndBattle.set_win(1);
	xReqAckEndBattle.set_star(2);
	xReqAckEndBattle.set_gold(nGambleGold);
	xReqAckEndBattle.set_exp(0);
	xReqAckEndBattle.set_diamond(nGambleDiamond);

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

	if (self == xViewOpponent)
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
	vKeyList.push_back(NFrame::Player::HeroPos1CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos1Star());
	vKeyList.push_back(NFrame::Player::HeroPos2CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos2Star());
	vKeyList.push_back(NFrame::Player::HeroPos3CnfID());
	vKeyList.push_back(NFrame::Player::HeroPos3Star());

	//try
	const std::string& strPlayerKey = m_pCommonRedisModule->GetPropertyCacheKey(opponent);
	if (m_pNoSqlModule->HMGet(strPlayerKey, vKeyList, vValueList))
	{
		int nGold = atoi(vValueList.at(0).c_str());
		int nDiamond = atoi(vValueList.at(1).c_str());
		int nLevel = atoi(vValueList.at(2).c_str());
		int nCup = atoi(vValueList.at(3).c_str());
		std::string strName = vValueList.at(4);
		std::string strHero1CnfID = vValueList.at(5);
		int nHero1Star = atoi(vValueList.at(6).c_str());
		std::string strHero2CnfID = vValueList.at(7);
		int nHero2Star = atoi(vValueList.at(7).c_str());
		std::string strHero3CnfID = vValueList.at(9);
		int nHero3Star = atoi(vValueList.at(10).c_str());


		return true;
	}

	return false;
}

bool NFCPVPModule::SearchOpponent(const NFGUID & self)
{
	int nSceneID = RandomTileScene();
	std::string strTileData;
	NFGUID xViewOpponent;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, xViewOpponent, strTileData))
	{
		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOpponent(), xViewOpponent);
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightOpponent(), NFGUID());

			m_pSceneProcessModule->RequestEnterScene(self, nSceneID, 0, NFDataList());

			//process opponent data
			ProcessOpponentData(self, xViewOpponent);

			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(nSceneID);
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, self);

			//create the building of the oppnent

			return true;
		}
		else
		{

			return false;
		}
	}

	return false;
}

int NFCPVPModule::RandomTileScene()
{
	return mxTileSceneIDList.at(m_pKernelModule->Random(0, (int)mxTileSceneIDList.size()));
}

void NFCPVPModule::OnReqAddGambleProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}
