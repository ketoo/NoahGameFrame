// -------------------------------------------------------------------------
//    @FileName      :    NFCGameLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameLogicModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCGameLogicModule::Init()
{
    return true;
}

bool NFCGameLogicModule::Shut()
{

    return true;
}

bool NFCGameLogicModule::Execute()
{

    return true;
}

bool NFCGameLogicModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogicClassModule );

    return true;
}

bool NFCGameLogicModule::BeforeShut()
{

    return true;
}
