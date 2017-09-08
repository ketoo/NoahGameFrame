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

    virtual NFIRankModule::RankValue GetIndex(const NFGUID& self, const NFIRankModule::RANK_TYPE type);

    virtual int RangeByIndex(const NFINT64 startIndex, const NFINT64 endIndex, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);
    virtual int RangeByScore(const NFINT64 startScore, const NFINT64 endScore, const NFIRankModule::RANK_TYPE type, std::vector<NFIRankModule::RankValue>& vector);

private:
    std::string MakeRanKey(const NFIRankModule::RANK_TYPE type);

private:
    NFIKernelModule* m_pKernelModule;
    NFINoSqlModule* m_pNoSqlModule;
};

#endif
