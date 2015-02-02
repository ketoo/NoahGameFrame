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
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
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

bool NFCGameLogicModule::Execute( const float fLasFrametime, const float fStartedTime )
{

    return true;
}

bool NFCGameLogicModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    //m_pNoSqlModule = dynamic_cast<NFIDataNoSqlModule*>(pPluginManager->FindModule("NFCDataNoSqlModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    //assert(NULL != m_pNoSqlModule);
    assert( NULL != m_pLogicClassModule );

    return true;
}

bool NFCGameLogicModule::BeforeShut()
{

    return true;
}
