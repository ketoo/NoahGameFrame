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

#ifndef NF_GAME_SERVER_TO_WORLD_MODULE_H
#define NF_GAME_SERVER_TO_WORLD_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"

class NFGameServerToWorldModule : public NFIGameServerToWorldModule
{
public:
    NFGameServerToWorldModule(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
		mLastReportTime = 0;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void TransmitToWorld(const int nHashKey, const int nMsgID, const google::protobuf::Message& xData);

	virtual void SendOnline(const NFGUID& self);
	virtual void SendOffline(const NFGUID& self);

protected:

    void OnSocketWSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

protected:
    void Register(NFINet* pNet);
	void ServerReport();
    void TransPBToProxy(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	
	void OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFINT64 mLastReportTime;

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFIElementModule* m_pElementModule;
	NFINetClientModule* m_pNetClientModule;
    NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif