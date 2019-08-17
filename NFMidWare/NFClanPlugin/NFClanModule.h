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


#ifndef NF_CLAN_MODULE_H
#define NF_CLAN_MODULE_H

#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClanModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClanRedisModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFClanModule
    : public NFIClanModule
{
public:
    NFClanModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    NFGUID CreateClan(const std::string& strClanName, const std::string& strClanDesc);

	bool AddMember(const NFGUID& xClanID, const NFGUID& player, const std::string& strPlayerName, const int bp, const MEMBER_TYPE type);
    bool LeaveClan(const NFGUID& xClanID, const NFGUID& self);

    bool PromotionMember(const NFGUID& xClanID, const NFGUID& self, const NFGUID& xMember);
    bool DemotionMember(const NFGUID& xClanID, const NFGUID& self, const NFGUID& xMember);

    bool MemberOnline(const NFGUID& self, const NFGUID& xClan, const int bp);
    bool MemberOffline(const NFGUID& self, const NFGUID& xClan, const int bp);

    void OnClanOnlineProcess(const NFGUID& xClanID);
    void OnClanOfflineProcess(const NFGUID& xClanID);


	NFIClanModule::MEMBER_TYPE CheckPower(const NFGUID& xClanID, const NFGUID& self);

protected:

	void OnCreateClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprClanMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSearchClanProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

protected:
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

	void SendMessageToGameServer(const NFGUID& Clan, const int nMessageID, google::protobuf::Message& msg);

private:
    int mnMaxCount = 50;

protected:
	NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFIClanRedisModule* m_pClanRedisModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
};

#endif