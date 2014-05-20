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
#include "NFComm/NFCore/NFPlatform.h"

bool NFCWorldLogicModule::Init()
{
    mnWorldContranerID = - 3;
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

bool NFCWorldLogicModule::Execute(const float fLasFrametime, const float fStartedTime)
{

    //////////////////////////////////////////////////////////////////////////
    // #ifdef NF_DEBUG_MODE
    //     char szContent[MAX_PATH] = { 0 };
    //     if (kbhit() && gets(szContent))
    //     {
    //         NFCValueList val(szContent, ",");
    //         if (val.GetCount() > 0)
    //         {
    //             //const char* pstrCmd = val.StringVal( 0 );
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


    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    //m_pDataBaseModule = dynamic_cast<NFIDataBaseModule*>( pPluginManager->FindModule( "NFCDataBaseModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));


    assert(NULL != m_pEventProcessModule);
    //assert( NULL != m_pDataBaseModule );
    assert(NULL != m_pKernelModule);
    return true;
}
