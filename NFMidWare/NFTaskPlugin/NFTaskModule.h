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


#ifndef NF_TASK_MODULE_H
#define NF_TASK_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFTaskModule
    : public NFITaskModule
{
public:
    NFTaskModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFTaskModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
		int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );
		int OnPlayerLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
protected:
    void OnClientAcceptTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClientPushTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


private:
	NFILogModule*					m_pLogModule;
	NFINetModule*					m_pNetModule;
    NFIKernelModule* 				m_pKernelModule;
    NFIPackModule* 					m_pPackModule;
    NFIElementModule* 				m_pElementModule;
    NFIClassModule* 				m_pLogicClassModule;
    NFIPropertyModule* 				m_pPropertyModule;
	NFIHeroModule* 					m_pHeroModule;
	NFICommonConfigModule* 			m_pCommonConfigModule;
	NFIGameServerNet_ServerModule*	m_pGameServerNet_ServerModule;
};


#endif
