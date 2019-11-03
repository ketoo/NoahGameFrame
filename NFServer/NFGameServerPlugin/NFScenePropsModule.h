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

#ifndef NF_SCENE_PROPS_MODULE_H
#define NF_SCENE_PROPS_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIScenePropsModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFScenePropsModule
	: public NFIScenePropsModule
{
public:

	NFScenePropsModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFScenePropsModule() {};

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool Shut();
	virtual bool Execute();

protected:
	void ReqBuildings(const int sceneID, const NFVector3& pos);

private:
	int OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnObjectBuildingRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
	int OnPlayePositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar);
	
	void OnAckBuildingsProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen);
	
	int OnSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

private:
	NFILogModule* m_pLogModule;
	NFISceneModule* m_pSceneModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFINetModule* m_pNetModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFINetClientModule* m_pNetClientModule;

	std::map<NFGUID, bool> mCellPullState;
};

#endif