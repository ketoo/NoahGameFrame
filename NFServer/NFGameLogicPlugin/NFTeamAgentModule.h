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

#ifndef NF_TEAM_AGENT_MODULE_H
#define NF_TEAM_AGENT_MODULE_H

#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFITeamAgentModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFTeamAgentModule
    : public NFITeamAgentModule
{
public:

    NFTeamAgentModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFTeamAgentModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();



protected:

   int OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var);

protected:
	
	void OnAckTeammateListProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnAckTeamInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnAckPlayerAddProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnAckPlayerLeaveProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

protected:

	void OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqCancelInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);


private:
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFINetClientModule* m_pNetClientModule;
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
};


#endif
