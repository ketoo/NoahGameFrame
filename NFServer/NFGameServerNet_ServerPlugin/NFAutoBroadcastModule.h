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

#ifndef NF_AUTO_BROADCAST_MODULE_H
#define NF_AUTO_BROADCAST_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
////////////////////////////////////////////////////////////////////////////

class NFIAutoBroadcastModule
    : public NFIModule
{
public:

};

class NFAutoBroadcastModule
    : public NFIAutoBroadcastModule
{
public:
	NFAutoBroadcastModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

private:
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	int OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	//broad the data of argvar to self
	int OnObjectListEnter(const NFDataList& self, const NFDataList& argVar);
	int OnObjectDataFinished(const NFDataList& self, const NFDataList& argVar);
	int OnObjectListLeave(const NFDataList& self, const NFDataList& argVar);

	//broad the data of self to argvar 
	int OnPropertyEnter(const NFDataList& argVar, const NFGUID& self);
	int OnRecordEnter(const NFDataList& argVar, const NFGUID& self);

	int OnPropertyEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);
	int OnRecordEvent(const NFGUID& self, const std::string& strRecord, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);


private:

    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneModule* m_pSceneModule;
    NFINetClientModule* m_pNetClientModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};
#endif
