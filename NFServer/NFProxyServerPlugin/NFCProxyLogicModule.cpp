// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

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

bool NFCProxyLogicModule::Execute()
{


    return true;
}


bool NFCProxyLogicModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();

    return true;
}
