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

bool NFCSLGModule::Execute()
{
    return true;
}

bool NFCSLGModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );

    assert( NULL != m_pKernelModule );

    return true;
}

bool NFCSLGModule::BeforeShut()
{
    return true;
}
