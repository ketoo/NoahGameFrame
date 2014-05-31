// -------------------------------------------------------------------------
//    @FileName      :    NFCPythonScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCPythonScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCPythonScriptModule.h"
#include "NFPythonScriptPlugin.h"

bool NFCPythonScriptModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);


    return true;
}

bool NFCPythonScriptModule::AfterInit()
{


    return true;
}

bool NFCPythonScriptModule::Shut()
{

    return true;
}

bool NFCPythonScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}
