/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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


#ifndef NF_NPC_REFRESH_MODULE_H
#define NF_NPC_REFRESH_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"

class NFINPCRefreshModule : public NFIModule
{

};

class NFNPCRefreshModule
    : public NFINPCRefreshModule
{
public:
    NFNPCRefreshModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFNPCRefreshModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
    int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );

    int OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

	int OnNPCDeadDestroyHeart(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);
	int OnBuildingDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
	int OnObjectBeKilled( const NFGUID& self, const NFGUID& killer );

private:
	NFIEventModule* m_pEventModule;
	NFIScheduleModule* m_pScheduleModule;
    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFISceneProcessModule* m_pSceneProcessModule;
	NFILogModule* m_pLogModule;
	NFIPropertyModule* m_pPropertyModule;
    NFISceneModule* m_pSceneModule;
};


#endif
