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


#ifndef NF_RANK_MODULE_H
#define NF_RANK_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIRankModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFRankModule : public NFIRankModule
{
public:
    NFRankModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFRankModule() {}

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void AddValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value);
    virtual void SetValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value);
    virtual void SubValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type, NFINT64 value);
    virtual void RemoveValue(const NFGUID& self, const NFIRankModule::RANK_TYPE type);

    virtual NFIRankModule::RankValue GetIndex(const NFGUID& self, const NFIRankModule::RANK_TYPE type);

    virtual int RangeByIndex(const NFINT64 startIndex, const NFINT64 endIndex, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);
    virtual int RangeByScore(const NFINT64 startScore, const NFINT64 endScore, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);

    virtual int GetRankListCount(const NFIRankModule::RANK_TYPE type);

    //for area
    virtual void AddValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value);
    virtual void SetValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value);
    virtual void SubValue(const int64_t nAreaID, const NFGUID& self, const RANK_TYPE type, NFINT64 value);
    virtual void RemoveValue(const int64_t nAreaID, const NFGUID& self, const NFIRankModule::RANK_TYPE type);

    virtual NFIRankModule::RankValue GetIndex(const int64_t nAreaID, const NFGUID& self, const NFIRankModule::RANK_TYPE type);

    virtual int RangeByIndex(const int64_t nAreaID, const NFINT64 startIndex, const NFINT64 endIndex, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);
    virtual int RangeByScore(const int64_t nAreaID, const NFINT64 startScore, const NFINT64 endScore, const RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);

    virtual int GetRankListCount(const int64_t nAreaID, const NFIRankModule::RANK_TYPE type);


private:
    std::string MakeRanKey(const RANK_TYPE type, const int64_t nAreaID);

private:
    NFIKernelModule* m_pKernelModule;
    NFINoSqlModule* m_pNoSqlModule;
};

#endif
