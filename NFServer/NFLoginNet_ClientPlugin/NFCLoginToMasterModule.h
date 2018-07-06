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

#ifndef NFC_LOGINNET_CLIENT_MODULE_H
#define NFC_LOGINNET_CLIENT_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILoginToMasterModule.h"

class NFCLoginToMasterModule
    : public NFILoginToMasterModule
{
public:
    NFCLoginToMasterModule(NFIPluginManager* p)
    {
        pPluginManager = p;
		mLastReportTime = 0;
    }


    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

	virtual NFINetClientModule* GetClusterModule();
    virtual NFMapEx<int, NFMsg::ServerInfoReport>& GetWorldMap();

protected:
    void OnSocketMSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

protected:

    //////////////////////////////////////////////////////////////////////////
	void OnSelectServerResultProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnWorldInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////
    void Register(NFINet* pNet);
	void ServerReport();

private:
	NFINT64 mLastReportTime;
    NFMapEx<int, NFMsg::ServerInfoReport> mWorldMap;

    NFILoginNet_ServerModule* m_pLoginNet_ServerModule;
    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
	NFINetClientModule* m_pNetClientModule;
};

#endif