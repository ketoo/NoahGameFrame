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
#include "NFWorldPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFWorldPVPModule::Init()
{
	
    return true;
}


bool NFWorldPVPModule::Shut()
{
    return true;
}

bool NFWorldPVPModule::Execute()
{
    
    return true;
}

bool NFWorldPVPModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFWorldPVPModule::OnReqSearchOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ADD_BUILDING, this, &NFWorldPVPModule::OnReqAddBuildingProcess)) { return false; }

    return true;
}

bool NFWorldPVPModule::ReadyExecute()
{

	return false;
}

void NFWorldPVPModule::OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT( nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);

	int nSearchCount = 0;
	bool bRet = false;
	while (!bRet && nSearchCount <= 5)
	{
		bRet = SearchOpponent(nPlayerID, xMsg.self_scene(), nSockIndex);
		nSearchCount++;
	}

	if (!bRet)
	{
		//failed
		NFMsg::AckSearchOppnent xAckData;
		xAckData.set_scene_id(0);
		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nSockIndex);

		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "ERROR TO FIND A OPPNENT!", "", __FUNCTION__, __LINE__);
	}
}

void NFWorldPVPModule::OnReqAddBuildingProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAddSceneBuilding);

	NFGUID posCell(xMsg.pos().x() / 100, xMsg.pos().z() / 100);
	//store it by (position.x / cellWidth=100, position.z / cellWidth=100)
	const std::string& strSceneKey = m_pCommonRedisModule->GetCellCacheKey(xMsg.scene_id());
	const std::string& strCellKey = posCell.ToString();
	const std::string& strCellValue = posCell.ToString() + "_CellInfo";
	NF_SHARE_PTR<NFIRedisClient> xRedisClient = m_pNoSqlModule->GetDriverBySuit(strSceneKey);
	if (xRedisClient)
	{
		//scene_id -> cell_id ->cell_id_ref -> cell_building_list
		xRedisClient->HSET(strSceneKey, strCellKey, strCellValue);
		xRedisClient->LPUSH(strCellValue, xMsg.SerializeAsString());
	}
}

void NFWorldPVPModule::OnReqBuildingsProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqSceneBuildings);

	NFGUID posCell(xMsg.pos().x() / 100, xMsg.pos().z() / 100);
	//store it by (position.x / cellWidth=100, position.z / cellWidth=100)
	const std::string& strSceneKey = m_pCommonRedisModule->GetCellCacheKey(xMsg.scene_id());
	const std::string& strCellKey = posCell.ToString();
	NF_SHARE_PTR<NFIRedisClient> xRedisClient = m_pNoSqlModule->GetDriverBySuit(strSceneKey);
	if (xRedisClient)
	{
		//scene_id -> cell_id ->cell_id_ref -> cell_building_list
		std::string strCellValue;
		if (xRedisClient->HGET(strSceneKey, strCellKey, strCellValue))
		{
			std::vector<std::string> vector_string;
			xRedisClient->LRANGE(strCellValue, 0, -1, vector_string);

			if (vector_string.size() > 0)
			{
				NFMsg::AckSceneBuildings ackSceneBuildings;
				for (int i = 0; i < vector_string.size(); ++i)
				{
					const std::string& data = vector_string[i];

				}
			}
		}
	}
}

bool NFWorldPVPModule::SearchOpponent(const NFGUID & self, const int nExceptSceneID, const NFSOCK nSockIndex)
{
	//search in the scene where the player at
	int nSceneID = 1;// RandomTileScene(nExceptSceneID);
	std::string strTileData;
	NFGUID xViewOpponent;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, xViewOpponent, strTileData) && !xViewOpponent.IsNull())
	{
		if (xViewOpponent == self)
		{
			return false;
		}

		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			NFMsg::AckSearchOppnent xAckData;
			xAckData.mutable_title()->CopyFrom(xTileData);
			xAckData.set_scene_id(nSceneID);

			ProcessOpponentData(xViewOpponent, xAckData);

			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nSockIndex, self);

			return true;
		}
		else
		{

			return false;
		}
	}

	return false;
}


bool NFWorldPVPModule::ProcessOpponentData(const NFGUID& opponent, NFMsg::AckSearchOppnent& xAckData)
{
	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;

	//0-11
	vKeyList.push_back(NFrame::Player::Gold());
	vKeyList.push_back(NFrame::Player::Diamond());
	vKeyList.push_back(NFrame::Player::Level());
	vKeyList.push_back(NFrame::Player::Cup());
	vKeyList.push_back(NFrame::Player::Name());
	vKeyList.push_back(NFrame::Player::Head());
	vKeyList.push_back(NFrame::Player::HeroCnfID1());
	vKeyList.push_back(NFrame::Player::HeroStar1());
	vKeyList.push_back(NFrame::Player::HeroCnfID2());
	vKeyList.push_back(NFrame::Player::HeroStar2());
	vKeyList.push_back(NFrame::Player::HeroCnfID3());
	vKeyList.push_back(NFrame::Player::HeroStar3());

	//12-17
	vKeyList.push_back(NFrame::Player::HeroID1());
	vKeyList.push_back(NFrame::Player::HeroID2());
	vKeyList.push_back(NFrame::Player::HeroID3());
	//vKeyList.push_back(NFrame::Player::HeroPos1());
	//vKeyList.push_back(NFrame::Player::HeroPos2());
	//vKeyList.push_back(NFrame::Player::HeroPos3());
	//try
	const std::string& strPlayerKey = m_pCommonRedisModule->GetPropertyCacheKey(opponent);
	NF_SHARE_PTR<NFIRedisClient> xRedisClient = m_pNoSqlModule->GetDriverBySuit(strPlayerKey);
	if (xRedisClient)
	{
		if (xRedisClient->HMGET(strPlayerKey, vKeyList, vValueList))
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

			NFGUID xHero1(vValueList.at(12));
			NFGUID xHero2(vValueList.at(13));
			NFGUID xHero3(vValueList.at(14));
			//NFVector3 vec1(vValueList.at(15));
			//NFVector3 vec2(vValueList.at(16));
			//NFVector3 vec3(vValueList.at(17));

			/////
			xAckData.set_gold(nGold);
			xAckData.set_diamond(nDiamond);
			xAckData.set_level(nLevel);
			xAckData.set_cup(nCup);
			xAckData.set_name(strName);
			xAckData.set_head(strHead);
			xAckData.set_hero_cnf1(strHero1CnfID);
			xAckData.set_hero_cnf2(strHero2CnfID);
			xAckData.set_hero_cnf3(strHero3CnfID);
			xAckData.set_hero_star1(nHero1Star);
			xAckData.set_hero_star2(nHero2Star);
			xAckData.set_hero_star3(nHero3Star);

			xAckData.mutable_hero_id1()->CopyFrom(NFINetModule::NFToPB(xHero1));
			xAckData.mutable_hero_id2()->CopyFrom(NFINetModule::NFToPB(xHero2));
			xAckData.mutable_hero_id3()->CopyFrom(NFINetModule::NFToPB(xHero3));

			xAckData.mutable_hero_pos1()->CopyFrom(NFINetModule::NFToPB(NFVector3()));
			xAckData.mutable_hero_pos2()->CopyFrom(NFINetModule::NFToPB(NFVector3()));
			xAckData.mutable_hero_pos3()->CopyFrom(NFINetModule::NFToPB(NFVector3()));

			xAckData.mutable_opponent()->CopyFrom(NFINetModule::NFToPB(opponent));

			std::ostringstream stream;
			//stream << " searcher: " << strHero1CnfID;
			stream << " name: " << strName;
			stream << " cup: " << nCup;
			stream << " level: " << nLevel;
			stream << " diamond: " << nDiamond;
			stream << " gold: " << nGold;
			stream << " hero1: " << strHero1CnfID;
			//stream << " pos: " << vec1.ToString();
			stream << " hero2: " << strHero2CnfID;
			//stream << " pos: " << vec2.ToString();
			stream << " hero3: " << strHero3CnfID;
			//stream << " pos: " << vec3.ToString();

			m_pLogModule->LogInfo(stream);
			return true;
		}
	}

	return false;
}