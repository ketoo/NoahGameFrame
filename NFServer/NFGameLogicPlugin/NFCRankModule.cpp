#include "NFCRankModule.h"

bool NFCRankModule::Init()
{
    return true;
}

bool NFCRankModule::Shut()
{
    return true;
}

bool NFCRankModule::Execute()
{
    return true;
}

bool NFCRankModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");

    assert(NULL != m_pKernelModule);

    return true;
}

int NFCRankModule::OnReqRankList(const NFGUID& self, const int nRankType, const int nStart, const int nEnd)
{
    return 0;
}

void NFCRankModule::LoadRankList()
{
    // TODO
}

void NFCRankModule::ProcessRankValueUpdate(const NFGUID& self, const int nRankType, const int nOperateType, const NFINT64 nValue)
{
    // TODO
}