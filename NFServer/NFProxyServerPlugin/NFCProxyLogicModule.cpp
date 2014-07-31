// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCProxyPlugin.h"
#include "NFCProxyLogicModule.h"
#include "NFComm/Config/NFConfig.h"

bool NFCProxyLogicModule::Init()
{



    return true;
}

bool NFCProxyLogicModule::Shut()
{
    return true;
}

bool NFCProxyLogicModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    // #ifdef NF_DEBUG_MODE
    //     char szContent[MAX_PATH] = { 0 };
    //     if (kbhit() && gets(szContent))
    //     {
    //         NFCDataList val(szContent, ",");
    //         if (val.GetCount() > 0)
    //         {
    //             //const char* pstrCmd = val.String( 0 );
    //             m_pKernelModule->Command(val);
    //
    //         }
    //     }
    // #endif


    return true;
}


bool NFCProxyLogicModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    //m_pShareMemoryModule = dynamic_cast<NFIShareMemoryModule*>(pPluginManager->FindModule("NFCShareMemoryModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    //assert(NULL != m_pShareMemoryModule);
    assert(NULL != m_pLogicClassModule);
    return true;
}
