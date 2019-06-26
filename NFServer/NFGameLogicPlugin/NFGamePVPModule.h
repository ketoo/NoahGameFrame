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


#ifndef NF_GAME_PVP_MODULE_H
#define NF_GAME_PVP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGamePVPModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

class NFGamePVPModule
    : public NFIGamePVPModule
{
public:
	NFGamePVPModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFGamePVPModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();
	virtual bool ReadyExecute();


protected:
	void OnReqSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnAckSearchOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnAckCancelSearchProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqEndPVPOpponentProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

	void OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
protected:

	int EnterSceneConditionEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	int BeforeEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

protected:

	void ResetPVPData(const NFGUID & self);
	void RecordPVPData(const NFGUID & self, const int nStar, const int nGold, const int nDiamond);

	int OnNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnNPCHPEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	
	int OnMatchKillsEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnMatchDeathsEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

	int OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnPlayerHPEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

	void EndTheBattle(const NFGUID& self, const int autoEnd);

protected:

private:
	
	NFIScheduleModule* m_pScheduleModule;
	NFIHeroModule* m_pHeroModule;
	NFIPropertyModule* m_pPropertyModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFISceneModule* m_pSceneModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFILevelModule* m_pLevelModule;
	NFINetClientModule* m_pNetClientModule;
};


#endif
