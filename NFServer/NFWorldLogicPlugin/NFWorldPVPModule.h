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


#ifndef NF_WORLD_PVP_MODULE_H
#define NF_WORLD_PVP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIWorldPVPModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFITileModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIWorldPVPModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFWorldPVPModule
    : public NFIWorldPVPModule
{
public:
	NFWorldPVPModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFWorldPVPModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();
	virtual bool ReadyExecute();

	virtual void OffLine(const NFGUID& self);

protected:
	void OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqEndTheBattleProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	
protected:
	void InitAllTileScene();
	int RandomTileScene(const int nExceptSceneID);

	int OnMakeTeam(const std::string& strHeartBeat, const float fTime, const int nCount);
	int OnMakeSurvivalTeam(const int playerCount, const int robotCount);
	int OnMakeSurvivalTeam(const std::string& strHeartBeat, const float fTime, const int nCount);

	int OnMakeMatch(const std::string& strHeartBeat, const float fTime, const int nCount);

protected:

	std::vector<int> mxTileSceneIDList;


protected:
	class MultiTeam
	{
	public:
		NFMsg::EBattleType battleType;
		NFGUID leaderID;
		NFGUID teamID;
		int avgBattlePoint = 0;
		int diamond = 0;
		std::vector<NFGUID> members;
	};

	std::list<NFGUID> mSingleModeCandidatePool;
	std::list<NFGUID> mMultipleModeCandidatePool;
	std::list<NFGUID> mSurvivalModeCandidatePool;
	NFMapEx<NFGUID, NFMsg::PVPPlayerInfo> mCandidatePool;
	std::list<NF_SHARE_PTR<MultiTeam>> mTeamList;

private:
	NFIKernelModule* m_pKernelModule;
	NFIScheduleModule* m_pScheduleModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFILogModule* m_pLogModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFICommonRedisModule* m_pCommonRedisModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
};


#endif
