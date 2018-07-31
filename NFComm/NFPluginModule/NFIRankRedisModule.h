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


#ifndef NFI_RANK_REDIS_MODULE_H
#define NFI_RANK_REDIS_MODULE_H

#include "NFIModule.h"

class NFIRankRedisModule
    : public NFIModule
{

public:
	virtual bool SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value) = 0;
	virtual bool IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value) = 0;

	virtual bool RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID& self) = 0;
	virtual bool RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop) = 0;
	virtual bool RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax) = 0;

	virtual bool GetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, double& dwValue) = 0;
	virtual bool GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;
	virtual bool GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;

	virtual int GetRankMemberCount(const NFMsg::ERankType eRankType) = 0;
	virtual int GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax) = 0;

};

#endif