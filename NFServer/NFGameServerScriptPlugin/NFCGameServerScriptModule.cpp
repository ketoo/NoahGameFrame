// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerScriptModule.h"
#include "NFGameServerScriptPlugin.h"

bool NFCGameServerScriptModule::Init()
{
    
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);


    return true;
}

bool NFCGameServerScriptModule::AfterInit()
{


    return true;
}

bool NFCGameServerScriptModule::Shut()
{

    return true;
}

bool NFCGameServerScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}
