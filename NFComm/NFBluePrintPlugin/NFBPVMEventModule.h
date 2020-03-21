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

#ifndef NF_BLUE_PRINT_VIRTUAL_MACHINE_EVENT_MODULE_H
#define NF_BLUE_PRINT_VIRTUAL_MACHINE_EVENT_MODULE_H

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"
#include "NFComm/NFPluginModule/NFIBPVMEventModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

#include "BluePrintVM/NFBPVirtualMachine.h"

class NFBPVMEventModule
    : public NFIBPVMEventModule
{
public:
    NFBPVMEventModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFBPVMEventModule() {};

    bool Awake() override;
    bool Init() override;
    bool AfterInit() override;
    bool CheckConfig() override;
    bool ReadyExecute() override;
    bool Execute() override;
    bool BeforeShut() override;
    bool Shut() override;
    bool Finalize() override;
    bool OnReloadPlugin() override;

private:
	struct BluePrintBlockAction
	{
		//event -> <monitorID, handler>
		NFMapEx<int, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mGameEvent;
		NFMapEx<int, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mNetEvent;
        NFMapEx<int, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mNetMsgEvent;

        //className
        NFMapEx<std::string, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mGameObjectEvent;
        //NFMapEx<std::string, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mObjectPropEvent;
        //NFMapEx<std::string, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mObjectRecordEvent;
        //NFMapEx<std::string, NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>> mObjectSceneEvent;
	};

private:

    virtual bool UnRegisterAllCallBack(const NFGUID blockID) override;

	// Inherited via NFIBPVMEventModule
	virtual bool RegisterGameEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) override;

	virtual bool RegisterNetEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) override;

    virtual bool RegisterNetMsgEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) override;

    virtual bool RegisterGameObjectEventCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) override;

private:
    int OnEventCommonEvent(const NFGUID& self, const int eventID, const NFDataList& var);

    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

    void OnMessageEvent(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    //block id ->
	NFMapEx<NFGUID, BluePrintBlockAction> mBluePrintBlockAction;


private:
    NFIBluePrintModule* m_pBluePrintModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventModule* m_pEventModule;
    NFINetModule* m_pNetModule;

};


#endif
