// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatModule
//
// -------------------------------------------------------------------------

#include "NFWorldChatPlugin.h"
#include "NFCWorldChatModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

bool NFCWorldChatModule::Init()
{


    return true;
}

bool NFCWorldChatModule::Shut()
{
    return true;
}

bool NFCWorldChatModule::Execute()
{


    return true;
}

bool NFCWorldChatModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);

    return true;
}
