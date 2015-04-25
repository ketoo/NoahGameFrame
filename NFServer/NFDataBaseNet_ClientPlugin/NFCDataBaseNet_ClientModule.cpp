// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCDataBaseNet_ClientModule.h"
#include "NFDataBaseNet_ClientPlugin.h"

bool NFCDataBaseNet_ClientModule::Init()
{    
    return true;
}

bool NFCDataBaseNet_ClientModule::Shut()
{
    return true;
}

bool NFCDataBaseNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCDataBaseNet_ClientModule::AfterInit()
{
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));

    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pKernelModule);

    return true;
}

bool NFCDataBaseNet_ClientModule::BeforeShut()
{
    return true;
}