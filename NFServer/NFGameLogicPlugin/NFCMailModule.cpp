#include "NFCMailModule.h"

bool NFCMailModule::Init()
{
    return true;
}

bool NFCMailModule::Shut()
{
    return true;
}

bool NFCMailModule::Execute()
{
    return true;
}

bool NFCMailModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

    return true;
}

bool NFCMailModule::SendMail(const NFGUID& reciever, const int nMailType, const NFDataList& xItemList, const std::string& strNailContent/* = NULL_STR*/)
{
    return true;
}

int NFCMailModule::OnDrawMail(const NFGUID& self, const NFDataList& var)
{
    return 0;
}

int NFCMailModule::OnOpenMail(const NFGUID& self, const NFDataList& var)
{
    return 0;
}

int NFCMailModule::OnDeleteMail(const NFGUID& self, const NFDataList& var)
{
    return 0;
}