/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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


#ifndef NFC_MAP_MODULE_H
#define NFC_MAP_MODULE_H

#include "NFComm/NFPluginModule/NFIMapModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCMapModule
    : public NFIMapModule
{
public:
	NFCMapModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCMapModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:

	
	void ReqMapTitleInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqBigMapsInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////
	
	void ReqStation(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


	
	void ReqGetMapAward(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqLeaveMsgToMap(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////

	
	void ReqMapHunting(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqMapKingWar(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void EndMapHunting(const std::string& strTitleID);
	void EndMapKingWar(const std::string& strTitleID);

	void SetKingForGrid(const std::string& strTitleID, const NFGUID& xGuildID);
	void LeaveStation(const std::string& strTitleID, const NFGUID& xGuildID);
private:
	NFINetModule* m_pNetModule;
	//NFIBigMapRedisModule* m_pBigMapRedisModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	//NFIGuildRedisModule* m_pGuildRedisModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;


};


#endif
