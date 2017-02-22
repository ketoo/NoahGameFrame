// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#include "NFCProxyLogicPlugin.h"
#include "NFCProxyLogicModule.h"

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
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();

    return true;
}
