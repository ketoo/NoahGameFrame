// -------------------------------------------------------------------------
//    @FileName      :    NFCJavaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCJavaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCJavaScriptModule.h"
#include "NFJavaScriptPlugin.h"

bool NFCJavaScriptModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);


    return true;
}

bool NFCJavaScriptModule::AfterInit()
{


    return true;
}

bool NFCJavaScriptModule::Shut()
{

    return true;
}

bool NFCJavaScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}
