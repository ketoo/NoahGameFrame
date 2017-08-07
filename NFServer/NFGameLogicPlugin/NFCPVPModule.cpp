// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#include "NFCPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
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

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFCPVPModule::OnReqSearchOppnentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_HOME_SCENE, this, &NFCPVPModule::OnReqSwapHomeSceneProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_START_OPPNENT, this, &NFCPVPModule::OnReqStartPVPOppnentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_END_OPPNENT, this, &NFCPVPModule::OnReqEndPVPOppnentProcess)) { return false; }

    return true;
}

bool NFCPVPModule::ReadyExecute()
{
	m_pSceneAOIModule->RemoveSwapSceneEventCallBack();
	m_pSceneAOIModule->AddSwapSceneEventCallBack(this, &NFCPVPModule::OnSceneEvent);

	return false;
}



void NFCPVPModule::OnReqSearchOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);
	//find a tile map and swap scene

	int nSceneID = RandomTileScene();
	std::string strTileData;
	NFGUID xViewOppnent;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, xViewOppnent, strTileData))
	{
		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOppnent(), xViewOppnent);
			m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOppnent(), NFGUID());

			m_pSceneProcessModule->RequestEnterScene(nPlayerID, nSceneID, 0, NFDataList());

			//tell client u shoud adjust tile
			//m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, nPlayerID);
			//tell client u should load resources
			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(nSceneID);
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nPlayerID);

			//create the building of the oppnent

			return;
		}
		else
		{
			//failed
			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(0);
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nPlayerID);

		}
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "ERROR TO FIND A OPPNENT!", "",  __FUNCTION__, __LINE__);
}

void NFCPVPModule::OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckHomeScene);
	int nHomeSceneID = m_pKernelModule->GetPropertyInt32(nPlayerID, NFrame::Player::HomeSceneID());

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOppnent(), nPlayerID);
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOppnent(), NFGUID());

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

void NFCPVPModule::OnReqStartPVPOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckStartBattle);

	NFGUID xViewOppnent = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::ViewOppnent());
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOppnent(), xViewOppnent);

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

void NFCPVPModule::OnReqEndPVPOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqEndBattle);

	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::ViewOppnent(), NFGUID());
	m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::FightOppnent(), NFGUID());
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
	NFGUID xViewOppnent = m_pKernelModule->GetPropertyObject(self, NFrame::Player::ViewOppnent());

	NFMsg::ReqAckSwapScene xAckSwapScene;
	xAckSwapScene.set_scene_id(nSceneID);
	xAckSwapScene.set_transfer_type(NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL);
	xAckSwapScene.set_line_id(0);
	xAckSwapScene.set_x(vRelivePos.X());
	xAckSwapScene.set_y(vRelivePos.Y());
	xAckSwapScene.set_z(vRelivePos.Z());

	if (self == xViewOppnent)
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::ViewOppnent(), NFGUID());

		if (m_pTileModule->GetOnlinePlayerTileData(self, strTileData))
		{
			xAckSwapScene.set_data(strTileData.c_str(), strTileData.length());
		}
	}
	else
	{
		if (m_pPlayerRedisModule->LoadPlayerTileRandomCache(xViewOppnent, strTileData))
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

int NFCPVPModule::RandomTileScene()
{
	return mxTileSceneIDList.at(m_pKernelModule->Random(0, (int)mxTileSceneIDList.size()));
}
