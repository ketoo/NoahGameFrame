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

    virtual void TransmitToWorld(const int nHashKey, const int msgID, const google::protobuf::Message& xData);

	virtual void SendOnline(const NFGUID& self);
	virtual void SendOffline(const NFGUID& self);

protected:

    void OnSocketWSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

protected:
    void Register(NFINet* pNet);
	void ServerReport();
    void TransPBToProxy(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    int OnObjectClassEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var);
	
	void OnServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

protected:
	void OnWorldPropertyIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldPropertyFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldPropertyStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldPropertyObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldPropertyVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldPropertyVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void OnWorldRecordEnterProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len);
	void OnWorldAddRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRemoveRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldSwapRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnWorldRecordVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
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