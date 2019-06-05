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


#ifndef NF_RANK_REDIS_MODULE_H
#define NF_RANK_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIRankRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFRankRedisModule : public NFIRankRedisModule
{
public:
	NFRankRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	virtual bool SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value);
	virtual bool IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value);

	virtual bool RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID& self);
	virtual bool RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop);
	virtual bool RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax);

	virtual bool GetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, double& dwValue);
	virtual bool GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec);
	virtual bool GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec);

	virtual int GetRankMemberCount(const NFMsg::ERankType eRankType);
	virtual int GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax);

protected:
	std::string MakeRedisKey(const NFMsg::ERankType eRankType);

private:
	NFINoSqlModule* m_pNoSqlModule;

};


#endif
