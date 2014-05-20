// -------------------------------------------------------------------------
//    @FileName      :    NFCDataUIMonitorModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataUIMonitorModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCDataUIMonitorModule.h"
#include "NFDataUIMonitorPlugin.h"

extern NFIPluginManager* pPluginManager;

NFIKernelModule* NFCDataUIMonitorModule::m_pKernelModule = NULL;
NFIEventProcessModule* NFCDataUIMonitorModule::m_pEventProcessModule = NULL;
NFIDataUIMonitorModule* NFCDataUIMonitorModule::m_pThis = NULL;

bool NFCDataUIMonitorModule::Init()
{
    return true;
}

bool NFCDataUIMonitorModule::AfterInit()
{
    m_pThis = this;
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pEventProcessModule);


    return true;
}


bool NFCDataUIMonitorModule::Shut()
{
    return true;
}

bool NFCDataUIMonitorModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

