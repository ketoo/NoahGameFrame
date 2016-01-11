// -------------------------------------------------------------------------
//    @FileName      :    NFCMasterModle.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterModle
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
//#include <conio.h>
#include "NFCMasterModule.h"
#include "NFMasterServerPlugin.h"

bool NFCMasterModule::Init()
{

    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);




    return true;
}

bool NFCMasterModule::Shut()
{
    return true;
}

bool NFCMasterModule::Execute()
{
 

    return true;
}
