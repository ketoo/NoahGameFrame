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

#ifndef NF_DATANOSQL_MODULE_H
#define NF_DATANOSQL_MODULE_H

#include "NFRedisClient.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFNoSqlModule
	: public NFINoSqlModule
{
public:

	NFNoSqlModule(NFIPluginManager* p);
	virtual ~NFNoSqlModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();
	
	virtual bool Connect(const std::string& strIP, const int nPort, const std::string& strPass) { return false; };
	virtual bool Enable();
	virtual bool Busy();
	virtual bool KeepLive();


	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP);
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort);
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass);

	virtual NFList<std::string> GetDriverIdList();
	virtual NF_SHARE_PTR<NFIRedisClient> GetDriver(const std::string& strID);
	virtual NF_SHARE_PTR<NFIRedisClient> GetDriverBySuitRandom();
	virtual NF_SHARE_PTR<NFIRedisClient> GetDriverBySuitConsistent();
	virtual NF_SHARE_PTR<NFIRedisClient> GetDriverBySuit(const std::string& strHash);
	//virtual NF_SHARE_PTR<NFIRedisClient> GetDriverBySuit(const int nHash);
	virtual bool RemoveConnectSql(const std::string& strID);

protected:
	void CheckConnect();

protected:
	NFINT64 mLastCheckTime;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;

	NFConsistentHashMapEx<std::string, NFIRedisClient> mxNoSqlDriver;

};

#endif