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


#ifndef NF_BIGMAP_REDIS_MODULE_H
#define NF_BIGMAP_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClanRedisModule.h"

class NFBigMapRedisModule : public NFIBigMapRedisModule
{
public:

	NFBigMapRedisModule(NFIPluginManager* p);

	virtual ~NFBigMapRedisModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	virtual bool GetGridBaseInfo(const std::string& strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo);
	virtual bool GetGridBaseInfo(std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo);
	virtual bool GetGridBaseInfo(const std::vector<std::string>& strGridID, std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo);
	
	virtual bool GetGridLeaveMsgInfo(const std::string& strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList);
	virtual bool GetGridWarHistoryInfo(const std::string& strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList);
	virtual bool GetGridStationInfo(const std::string& strGridID, std::vector<NFMsg::GridClanBaseInfo>& xWarHistoryList);

	virtual bool SetGridBaseInfo(const std::string& strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo);
	
	virtual bool AddGridLeaveMsgInfo(const std::string& strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg);
	virtual bool AddGridWarHistoryInfo(const std::string& strGridID, const NFMsg::BigMapWarHistory& xWarHistory);
	
	virtual bool AddGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource);
	virtual bool RemoveGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource);

private:
	std::string GetGridBaseKey();
	std::string GetGridLeaveMsgKey(const std::string&strGridID);
	std::string GetGridWarHistoryKey(const std::string&strGridID);
	std::string GetGridStationHistoryKey(const std::string&strGridID);

private:
	NFINoSqlModule* m_pNoSqlModule;
	NFIElementModule* m_pElementModule;
	NFIClassModule* m_pLogicClassModule;
	NFIClanRedisModule* m_pClanRedisModule;
};


#endif
