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

#ifndef NFI_GAMESERVERNET_SERVERMODULE_H
#define NFI_GAMESERVERNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFIGameServerNet_ServerModule
    : public NFIModule
{
public:

    struct GateBaseInfo
    {
		enum ERoleStatus
		{
			E_LOADING = 1,
			E_LOADED = 2,
		};

        GateBaseInfo()
        {
            nActorID = 0;
            gateID = 0;
			eStatus = E_LOADING;
        }

        GateBaseInfo(const int gateID, const NFGUID xIdent)
        {
            this->nActorID = 0;
			this->gateID = gateID;
			this->xClientID = xIdent;
        }

        int nActorID;
		int gateID;
		ERoleStatus eStatus;
        NFGUID xClientID;
    };

    struct GateServerInfo
    {
        ServerData xServerData;

        std::map<NFGUID, NFSOCK> xRoleInfo;
    };

public:

    //virtual void SendMsgPBToGate(const uint16_t msgID, const std::string& xMsg, const NFGUID& self) = 0;
    virtual void SendMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const NFGUID& self) = 0;
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const int sceneID, const int groupID) = 0;
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const int sceneID, const int groupID, const NFGUID exceptID) = 0;

	virtual void SendMsgToGate(const uint16_t msgID, const std::string& msg, const NFGUID& self) = 0;
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, const std::string& msg, const int sceneID, const int groupID) = 0;
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, const std::string& msg, const int sceneID, const int groupID, const NFGUID exceptID) = 0;


    virtual bool AddPlayerGateInfo(const NFGUID& roleID, const NFGUID& clientID, const int gateID) = 0;
    virtual bool RemovePlayerGateInfo(const NFGUID& roleID) = 0;

    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& roleID) = 0;
	virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int gateID) = 0;
	virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfoBySockIndex(const NFSOCK sockIndex) = 0;
};

#endif