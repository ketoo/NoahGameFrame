// -------------------------------------------------------------------------
//    @FileName			:    NFCWorldLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFWorldLogicPlugin.h"
#include "NFCWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

bool NFCWorldLogicModule::Init()
{

    return true;
}

bool NFCWorldLogicModule::Shut()
{
    return true;
}

bool NFCWorldLogicModule::Execute()
{


    return true;
}


bool NFCWorldLogicModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

    return true;
}
