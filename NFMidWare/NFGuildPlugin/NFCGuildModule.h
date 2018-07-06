/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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


#ifndef NFC_GUILD_MODULE_H
#define NFC_GUILD_MODULE_H

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCGuildModule
    : public NFIGuildModule
{
public:
    NFCGuildModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    bool CreateGuild(const NFGUID& self, const NFGUID& xGuildID, const std::string& strGuildName);
    bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID);
    bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID);
    bool PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
    bool DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);

    bool MemberOnline(const NFGUID& self, const NFGUID& xGuild);
    bool MemberOffline(const NFGUID& self, const NFGUID& xGuild);


    void OnGuildOnlineProcess(const NFGUID& xGuildID);
    void OnGuildOfflineProcess(const NFGUID& xGuildID);

	bool AddMember(const NFGUID& xGuildID, const NFGUID& player, const MEMBER_TYPE type);

    NFIGuildModule::MEMBER_TYPE CheckPower(const NFGUID& self, const NFGUID& xGuildID);

protected:

	void OnCreateGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprGuildMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSearchGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

protected:
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

	void SendMessageToGameServer(const NFGUID& guild, const int nMessageID, google::protobuf::Message& msg);

protected:
	NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
};

#endif