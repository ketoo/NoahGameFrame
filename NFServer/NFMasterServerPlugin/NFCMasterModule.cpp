// -------------------------------------------------------------------------
//    @FileName			:    NFCMasterModle.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterModle
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCMasterModule.h"
#include "NFMasterServerPlugin.h"

bool NFCMasterModule::Init()
{

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

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
