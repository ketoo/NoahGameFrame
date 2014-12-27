#include "NFSLGPlugin.h"
#include "NFCSLGModule.h"
#include "NFComm/Config/NFConfig.h"


bool NFCSLGModule::Init()
{
    return true;
}

bool NFCSLGModule::Shut()
{

    return true;
}

bool NFCSLGModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCSLGModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pNoSqlModule = dynamic_cast<NFIDataNoSqlModule*>( pPluginManager->FindModule( "NFCDataNoSqlModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    //assert( NULL != m_pNoSqlModule );

    return true;
}

bool NFCSLGModule::BeforeShut()
{
    return true;
}
