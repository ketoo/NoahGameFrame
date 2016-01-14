// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFWorldLogicPlugin.h"
#include "NFCWorldLogicModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

bool NFCWorldLogicModule::Init()
{
//     NFConfig config;
//     bool bLoad =  config.Load("NFWorldConfig.cfg");
//     if (!bLoad)
//     {
//         assert(0);
// 
//         return true;
//     }
// 
//     mnWorldID = config["WorldID"].toNumber<int>();
//     mstrWorldName = config["WorldName"].str();

    return true;
}

bool NFCWorldLogicModule::Shut()
{
    return true;
}

bool NFCWorldLogicModule::Execute()
{

    //////////////////////////////////////////////////////////////////////////
    // #ifdef NF_DEBUG_MODE
    //     char szContent[MAX_PATH] = { 0 };
    //     if (kbhit() && gets(szContent))
    //     {
    //         NFCDataList val(szContent, ",");
    //         if (val.GetCount() > 0)
    //         {
    //             //const char* pstrCmd = val.String( 0 );
    //             m_pKernelModule->Command(val);
    //         }
    //     }
    // #endif


    return true;
}

// int NFCWorldLogicModule::GetWorldID()
// {
//     return mnWorldID;
// }
// 
// const char* NFCWorldLogicModule::GetWorldName()
// {
//     return mstrWorldName.c_str();
// }

bool NFCWorldLogicModule::AfterInit()
{


    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    return true;
}
