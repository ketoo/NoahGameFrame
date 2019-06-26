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


#ifndef NF_SYNC_MODULE_H
#define NF_SYNC_MODULE_H

#include "NFComm/NFPluginModule/NFITileMapModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClanRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISyncModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFSyncModule
    : public NFISyncModule
{
public:
	NFSyncModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFSyncModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	int SyncHeart(const std::string& strHeartName, const float fTime, const int nCount);

	int OnNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnNPCPositionEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

	int OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnPlayePositionPEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

	void OnReqPosSyncProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

private:
	//sceneID -> <PlayerID, Position>
	NFMapEx<int, NFMap<NFGUID, NFVector3>> mxPlayerPosition;

private:
	NFIScheduleModule* m_pScheduleModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIKernelModule* m_pKernelModule;
	NFIElementModule* m_pElementModule;
	NFISceneModule* m_pSceneModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
