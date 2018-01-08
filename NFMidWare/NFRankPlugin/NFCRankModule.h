//------------------------------------------------------------------------ -
//    @FileName			:    NFRankPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFRankPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_RANK_MODULE_H
#define NFC_RANK_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIRankModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFCRankModule : public NFIRankModule
{
public:
    NFCRankModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCRankModule() {}

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
