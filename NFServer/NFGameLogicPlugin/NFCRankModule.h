// -------------------------------------------------------------------------
//    @FileName         :   NFCRankModule.h
//    @Author           :   NickYang
//    @Date             :   2012/12/15
//    @Module           :   NFCRankModule
//
// -------------------------------------------------------------------------

#ifndef NFC_RANK_MODULE_H
#define NFC_RANK_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIRankModule.h"

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

public:
    virtual int OnReqRankList(const NFGUID& self, const int nRankType, const int nStart, const int nEnd);

    virtual void LoadRankList();
    virtual void ProcessRankValueUpdate(const NFGUID& self, const int nRankType, const int nOperateType, const NFINT64 nValue);

private:
    NFIKernelModule* m_pKernelModule;
};

#endif // !NFC_RANK_MODULE_H
