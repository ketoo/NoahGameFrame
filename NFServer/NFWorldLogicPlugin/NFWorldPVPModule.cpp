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
#include "NFWorldPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "../NFDBLogicPlugin/NFPlayerRedisModule.h"

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
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	

	m_pScheduleModule->AddSchedule("DoMakeTeam", this, &NFWorldPVPModule::OnMakeTeam, 1.0f, -1);
	m_pScheduleModule->AddSchedule("DoMakeMatch", this, &NFWorldPVPModule::OnMakeMatch, 0.7f, -1);
	m_pScheduleModule->AddSchedule("OnMakeSurvivalTeam", this, &NFWorldPVPModule::OnMakeSurvivalTeam, 2, -1);
	
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFWorldPVPModule::OnReqSearchOpponentProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CANCEL_SEARCH, this, &NFWorldPVPModule::OnReqCancelSearchProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_END_OPPNENT, this, &NFWorldPVPModule::OnReqEndTheBattleProcess)) { return false; }

	InitAllTileScene();

    return true;
}

bool NFWorldPVPModule::ReadyExecute()
{

	return false;
}

void NFWorldPVPModule::OffLine(const NFGUID & self)
{
	NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo = mCandidatePool.GetElement(self);
	if (pvpPlayerInfo)
	{
		if (pvpPlayerInfo->battle_mode() == NFMsg::EBattleType::EBT_SINGLE_MODE)
		{
			mCandidatePool.RemoveElement(self);

			mSingleModeCandidatePool.remove(self);
		}
		else
		{

		}
	}
}

void NFWorldPVPModule::OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT( nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);

	if (xMsg.battle_mode() == NFMsg::EBattleType::EBT_SINGLE_MODE)
	{
		std::ostringstream stream;
		stream << nPlayerID.ToString() << " want to join a EBT_SINGLE_MODE match, mCandidatePool's size:" << mCandidatePool.Count();

		//single
		if (!mCandidatePool.ExistElement(nPlayerID))
		{
			NFMsg::PVPPlayerInfo pvpPlayerInfoTest;
			NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());

			NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
			if (pPlayerRedisModule->LoadPlayerData(nPlayerID, *pvpPlayerInfo))
			{
				pvpPlayerInfo->set_battle_mode(NFMsg::EBattleType::EBT_SINGLE_MODE);
				mSingleModeCandidatePool.push_back(nPlayerID);
				mCandidatePool.AddElement(nPlayerID, pvpPlayerInfo);


				stream << " -----> mCandidatePool's size:" << mCandidatePool.Count();
			}
		}

		m_pLogModule->LogInfo(stream);
	}
	else if (xMsg.battle_mode() == NFMsg::EBattleType::EBT_MULTI_MODE)
	{
		std::ostringstream stream;
		stream << nPlayerID.ToString() << " want to join a EBT_MULTI_MODE match, mCandidatePool's size:" << mCandidatePool.Count();

		if (xMsg.friends_size() == 0)
		{
			//play with stranger
			NFMsg::PVPPlayerInfo pvpPlayerInfoTest;
			NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());

			NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
			if (pPlayerRedisModule->LoadPlayerData(nPlayerID, *pvpPlayerInfo))
			{
				pvpPlayerInfo->set_battle_mode(NFMsg::EBattleType::EBT_MULTI_MODE);
				mMultipleModeCandidatePool.push_back(nPlayerID);
				mCandidatePool.AddElement(nPlayerID, pvpPlayerInfo);


				stream << " -----> mCandidatePool's size:" << mCandidatePool.Count();
			}

			m_pLogModule->LogInfo(stream);
		}
		else
		{
			//play with friends or clan members

			NF_SHARE_PTR<MultiTeam> multiTeam(NF_NEW MultiTeam());
			multiTeam->teamID = m_pKernelModule->CreateGUID();
			multiTeam->leaderID = nPlayerID;
			multiTeam->diamond = xMsg.diamond();
			multiTeam->battleType = NFMsg::EBattleType::EBT_MULTI_MODE;

			std::ostringstream stream;
			stream << nPlayerID.ToString() << " want to join a team mode match, mCandidatePool's size:" << mCandidatePool.Count();

			if (!mCandidatePool.ExistElement(nPlayerID))
			{
				NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());
				NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
				if (pPlayerRedisModule->LoadPlayerData(nPlayerID, *pvpPlayerInfo))
				{
					mCandidatePool.AddElement(nPlayerID, pvpPlayerInfo);
					multiTeam->members.push_back(nPlayerID);
				}
			}


			//multi-player
			int battlePoint = 0;
			if (xMsg.friends_size() <= 9)
			{
				for (int i = 0; i < xMsg.friends_size(); ++i)
				{
					NFGUID friendID = NFINetModule::PBToNF(xMsg.friends(i));
					if (!mCandidatePool.ExistElement(nPlayerID))
					{
						NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());
						NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
						if (pPlayerRedisModule->LoadPlayerData(friendID, *pvpPlayerInfo))
						{
							mCandidatePool.AddElement(friendID, pvpPlayerInfo);

							multiTeam->members.push_back(friendID);
							battlePoint += pvpPlayerInfo->battle_point();
						}
					}
				}
			}

			multiTeam->avgBattlePoint = battlePoint;
			mTeamList.push_back(multiTeam);

			stream << " -----> mCandidatePool's size:" << mCandidatePool.Count();
			m_pLogModule->LogInfo(stream);
		}
	}
	else if (xMsg.battle_mode() == NFMsg::EBattleType::EBT_SURVIVAL_MODE)
	{
		std::ostringstream stream;
		stream << nPlayerID.ToString() << " want to join a EBT_SURVIVAL_MODE match, mCandidatePool's size:" << mCandidatePool.Count();

		if (xMsg.friends_size() == 0)
		{
			//play with stranger
			NFMsg::PVPPlayerInfo pvpPlayerInfoTest;
			NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());

			NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
			if (pPlayerRedisModule->LoadPlayerData(nPlayerID, *pvpPlayerInfo))
			{
				pvpPlayerInfo->set_battle_mode(NFMsg::EBattleType::EBT_MULTI_MODE);
				mSurvivalModeCandidatePool.push_back(nPlayerID);
				mCandidatePool.AddElement(nPlayerID, pvpPlayerInfo);


				stream << " -----> mCandidatePool's size:" << mCandidatePool.Count();
			}

			m_pLogModule->LogInfo(stream);
		}
	}
}

void NFWorldPVPModule::OnReqCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCancelSearch);

	std::ostringstream stream;
	stream << nPlayerID.ToString() << " want to cancel a match search, mCandidatePool's size:" << mCandidatePool.Count();

	NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo = mCandidatePool.GetElement(nPlayerID);
	if (pvpPlayerInfo)
	{
		mSingleModeCandidatePool.remove(nPlayerID);
		mMultipleModeCandidatePool.remove(nPlayerID);
		mSurvivalModeCandidatePool.remove(nPlayerID);
		mCandidatePool.RemoveElement(nPlayerID);

		bool find = false;

		for (auto it = mTeamList.begin(); it != mTeamList.end(); ++it)
		{
			int size = (*it)->members.size();
			for (auto i = (*it)->members.begin(); i != (*it)->members.end(); ++i)
			{
				NFGUID id = *i;
				if (id == nPlayerID)
				{
					if (size > 1)
					{
						(*it)->members.erase(i);
						break;
					}
					else
					{
						find = true;
					}
				}
			}

			if (find)
			{
				mTeamList.erase(it);
				break;
			}
		}

		stream << " -----> mCandidatePool's size:" << mCandidatePool.Count();
		m_pLogModule->LogInfo(stream);

		m_pWorldNet_ServerModule->SendMsgToGame(nPlayerID, NFMsg::EGMI_ACK_CANCEL_SEARCH, xMsg);
	}
}

void NFWorldPVPModule::OnReqEndTheBattleProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckEndBattle);

}

int NFWorldPVPModule::RandomTileScene(const int nExceptSceneID)
{
	if (mxTileSceneIDList.size() > 1)
	{
		int index = m_pKernelModule->Random(0, (int)mxTileSceneIDList.size());
		if (index < mxTileSceneIDList.size())
		{
			int nSceneID = mxTileSceneIDList.at(index);
			if (nExceptSceneID != nSceneID)
			{
				return nSceneID;
			}
		}

		RandomTileScene(nExceptSceneID);
	}
	else
	{
		if (nExceptSceneID != mxTileSceneIDList[0])
		{
			return mxTileSceneIDList[0];
		}
	}

	return 0;
}

int NFWorldPVPModule::OnMakeTeam(const std::string & strHeartBeat, const float fTime, const int nCount)
{
	//single mode dosen't has gamble permission
	static int makeTeamCount = 0;
	if (mSingleModeCandidatePool.size() > 0
		|| mMultipleModeCandidatePool.size() > 0)
	{
		makeTeamCount++;
	}
	else
	{
		makeTeamCount = 0;
	}

	if (mSingleModeCandidatePool.size() > 0)
	{
		NFGUID id = mSingleModeCandidatePool.front();
		mSingleModeCandidatePool.pop_front();

		std::ostringstream stream;
		stream << " a team maked(EBT_SINGLE_MODE), mCandidatePool's size:" << mCandidatePool.Count();
		stream << " team member:" << id.ToString();

		NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo = mCandidatePool.GetElement(id);
		if (pvpPlayerInfo)
		{
			NF_SHARE_PTR<MultiTeam> teamData(NF_NEW MultiTeam());
			teamData->teamID = m_pKernelModule->CreateGUID();
			teamData->battleType = NFMsg::EBattleType::EBT_SINGLE_MODE;
			stream << " team id:" << teamData->teamID.ToString();


			teamData->members.push_back(id);
			teamData->avgBattlePoint = pvpPlayerInfo->battle_point();

			mTeamList.push_back(teamData);
			makeTeamCount = 0;

		}

		stream << " -----> team size:" << mTeamList.size() << " mCandidatePool's size:" << mCandidatePool.Count();
		m_pLogModule->LogInfo(stream, __FUNCTION__, __LINE__);
	}


	if (mMultipleModeCandidatePool.size() >= 5)
	{
		NF_SHARE_PTR<MultiTeam> teamData(NF_NEW MultiTeam());
		teamData->teamID = m_pKernelModule->CreateGUID();
		teamData->battleType = NFMsg::EBattleType::EBT_MULTI_MODE;

		std::ostringstream stream;
		stream << " a team maked(EBT_MULTI_MODE), mCandidatePool's size:" << mCandidatePool.Count();
		stream << " -----> team id:" << teamData->teamID.ToString() << " team member:";

		for (int i = 0; i < 5; ++i)
		{
			if (mMultipleModeCandidatePool.size() > 0)
			{
				NFGUID id = mMultipleModeCandidatePool.front();
				mMultipleModeCandidatePool.pop_front();

				teamData->members.push_back(id);

				stream << id.ToString() << ",";
			}
		}

		mTeamList.push_back(teamData);
		makeTeamCount = 0;

		stream << " -----> team size:" << mTeamList.size() << " mCandidatePool's size:" << mCandidatePool.Count();
		m_pLogModule->LogInfo(stream, __FUNCTION__, __LINE__);
	}
	else if (mMultipleModeCandidatePool.size() >= 3 && makeTeamCount >= 5)
	{
		NF_SHARE_PTR<MultiTeam> teamData(NF_NEW MultiTeam());
		teamData->teamID = m_pKernelModule->CreateGUID();
		teamData->battleType = NFMsg::EBattleType::EBT_MULTI_MODE;

		std::ostringstream stream;
		stream << " a team maked(EBT_MULTI_MODE), mCandidatePool's size:" << mCandidatePool.Count();
		stream << " -----> team id:" << teamData->teamID.ToString() << " team member:";

		for (int i = 0; i < 3; ++i)
		{
			if (mMultipleModeCandidatePool.size() > 0)
			{
				NFGUID id = mMultipleModeCandidatePool.front();
				mMultipleModeCandidatePool.pop_front();

				teamData->members.push_back(id);

				stream << id.ToString() << ",";
			}
		}

		mTeamList.push_back(teamData);
		makeTeamCount = 0;

		stream << " -----> team size:" << mTeamList.size() << " mCandidatePool's size:" << mCandidatePool.Count();
		m_pLogModule->LogInfo(stream, __FUNCTION__, __LINE__);
	}
	else if (mMultipleModeCandidatePool.size() >= 2 && makeTeamCount >= 7)
	{
		NF_SHARE_PTR<MultiTeam> teamData(NF_NEW MultiTeam());
		teamData->teamID = m_pKernelModule->CreateGUID();
		teamData->battleType = NFMsg::EBattleType::EBT_MULTI_MODE;

		std::ostringstream stream;
		stream << " a team maked(EBT_MULTI_MODE), mCandidatePool's size:" << mCandidatePool.Count();
		stream << " -----> team id:" << teamData->teamID.ToString() << " team member:";

		for (int i = 0; i < 2; ++i)
		{
			if (mMultipleModeCandidatePool.size() > 0)
			{
				NFGUID id = mMultipleModeCandidatePool.front();
				mMultipleModeCandidatePool.pop_front();

				teamData->members.push_back(id);

				stream << id.ToString() << ",";
			}
		}

		mTeamList.push_back(teamData);
		makeTeamCount = 0;

		stream << " -----> team size:" << mTeamList.size() << " mCandidatePool's size:" << mCandidatePool.Count();
		m_pLogModule->LogInfo(stream, __FUNCTION__, __LINE__);
	}

	return 0;
}

int NFWorldPVPModule::OnMakeMatch(const std::string & strHeartBeat, const float fTime, const int nCount)
{
	if (mTeamList.size() > 0)
	{
		NF_SHARE_PTR<MultiTeam> teamData = mTeamList.front();
		if (teamData)
		{
			mTeamList.pop_front();

			m_pLogModule->LogInfo(teamData->teamID.ToString() + " Team --> try to find a match", __FUNCTION__, __LINE__);

			int nSceneID = RandomTileScene(1);
			std::string strTileData;
			NFGUID xViewOpponent;
			if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, xViewOpponent, strTileData) && !xViewOpponent.IsNull())
			{
				for (int i = 0; i < teamData->members.size(); ++i)
				{
					if (teamData->members[i] == xViewOpponent)
					{
						m_pLogModule->LogError(teamData->teamID, "the same team member, push back and try to find a match again");
						mTeamList.push_back(teamData);
						return 0;
					}
				}

				std::ostringstream stream;
				stream << " a match maked, nSceneID:" << nSceneID << " ViewOpponent:" << xViewOpponent.ToString();
				stream << " team id:" << teamData->teamID.ToString();

				NFMsg::ReqStoreSceneBuildings xTileData;
				if (xTileData.ParseFromString(strTileData))
				{
					NFMsg::PVPPlayerInfo pvpPlayerInfoTest;
					NF_SHARE_PTR<NFMsg::PVPPlayerInfo> pvpPlayerInfo(NF_NEW NFMsg::PVPPlayerInfo());
					NFPlayerRedisModule* pPlayerRedisModule = (NFPlayerRedisModule*)m_pPlayerRedisModule;
					if (pPlayerRedisModule->LoadPlayerData(xViewOpponent, *pvpPlayerInfo))
					{
						NFMsg::AckSearchOppnent xAckData;
						*xAckData.mutable_team_id() = NFINetModule::NFToPB(teamData->teamID);
						xAckData.set_scene_id(nSceneID);
						xAckData.mutable_buildings()->CopyFrom(xTileData.buildings());
						xAckData.mutable_opponent()->CopyFrom(*pvpPlayerInfo);

						for (int i = 0; i < teamData->members.size(); ++i)
						{
							NFGUID id = teamData->members[i];
							
							stream << " team member:" << id.ToString();

							NFMsg::Ident* pIdent = xAckData.add_team_members();
							*pIdent = NFINetModule::NFToPB(id);
						}

						m_pLogModule->LogInfo(stream, __FUNCTION__, __LINE__);
						//must in the same game server (you would change the game server when you acceptted the invite)
						NFGUID id = teamData->members[0];
						m_pWorldNet_ServerModule->SendMsgToGame(id, NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData);
						//so we only send the message to the member[0]'s server
						/*
							for (int i = 0; i < teamData->members.size(); ++i)
							{
								NFGUID id = teamData->members[i];
								m_pWorldNet_ServerModule->SendMsgToGame(id, NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData);
							}
						*/
					}
				}
			}
			else
			{
				mTeamList.push_back(teamData);

				std::ostringstream stream;
				stream << " can't load the scene, nSceneID:" << nSceneID << " ViewOpponent:" << xViewOpponent.ToString();
				stream << " team id:" << teamData->teamID.ToString() << " push back and try to find a match again";

				m_pLogModule->LogError(stream);
				return 0;
			}
		}

		for (int i = 0; i < teamData->members.size(); ++i)
		{
			NFGUID id = teamData->members[i];
			mCandidatePool.RemoveElement(id);
		}
	}

	return 0;
}

int NFWorldPVPModule::OnMakeSurvivalTeam(const int playerCount, const int robotCount)
{
	if (playerCount > 0  && robotCount >= 0)
	{
		NF_SHARE_PTR<MultiTeam> teamData(NF_NEW MultiTeam());
		teamData->teamID = m_pKernelModule->CreateGUID();
		teamData->battleType = NFMsg::EBattleType::EBT_SURVIVAL_MODE;

		std::ostringstream stream;
		stream << " a team maked(EBT_SURVIVAL_MODE), mCandidatePool's size:" << mCandidatePool.Count();
		stream << " -----> team id:" << teamData->teamID.ToString() << " team member:";

		for (int i = 0; i < playerCount; ++i)
		{
			if (mSurvivalModeCandidatePool.size() > 0)
			{
				NFGUID id = mSurvivalModeCandidatePool.front();
				mSurvivalModeCandidatePool.pop_front();

				teamData->members.push_back(id);

				stream << id.ToString() << ",";
			}
		}

		//process robots

		mTeamList.push_back(teamData);
	}

	return 0;
}

int NFWorldPVPModule::OnMakeSurvivalTeam(const std::string & strHeartBeat, const float fTime, const int nCount)
{
	static int makeMatchCount = 0;
	makeMatchCount++;

	//match pool should come from BattleConfig.xml
	const int matchPool = 80;
	if (mSurvivalModeCandidatePool.size() >= matchPool)
	{
		OnMakeSurvivalTeam(matchPool, 0);
		
		//done
		makeMatchCount = 0;		
	}
	else
	{
		if (makeMatchCount > 10)
		{
			//wait for 20 senconds, would start the match with robots
			int playerCount = matchPool / 2;
			if (mSurvivalModeCandidatePool.size() >= playerCount)
			{

				//done
				makeMatchCount = 0;		
			}
		}
		else if (makeMatchCount > 20)
		{

			//wait for 40 senconds, would start the match with robots
			int playerCount = matchPool / 4;
			if (mSurvivalModeCandidatePool.size() >= playerCount)
			{
				//done
				makeMatchCount = 0;		
			}
		}
		else if (makeMatchCount > 25)
		{
			//wait for 50 senconds, would start the match with robots
			int playerCount = matchPool / 5;
			if (mSurvivalModeCandidatePool.size() >= playerCount)
			{
				//done
				makeMatchCount = 0;		
			}
		}
		else if (makeMatchCount > 30)
		{
			//wait for 60 senconds, would start the match with robots
			int playerCount = matchPool / 10;
			if (mSurvivalModeCandidatePool.size() >= playerCount)
			{
				OnMakeSurvivalTeam(playerCount, matchPool - playerCount);
				//done
				makeMatchCount = 0;		
			}
		}
	}

	return 0;
}

void NFWorldPVPModule::InitAllTileScene()
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

			const int nSceneType = m_pElementModule->GetPropertyInt32(strId, NFrame::Scene::Type());
			if (nSceneType == NFMsg::ESceneType::SCENE_HOME)
			{
				int nSceneID = lexical_cast<int>(strId);
				std::vector<int>::iterator it = std::find(mxTileSceneIDList.begin(), mxTileSceneIDList.end(), nSceneID);
				if (it == mxTileSceneIDList.end())
				{
					mxTileSceneIDList.push_back(nSceneID);
				}
			}
		}
	}

	//NFASSERT(0, "", __file__, __FUNCTION__);
}
