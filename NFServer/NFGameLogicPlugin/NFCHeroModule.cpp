// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCHeroModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCHeroModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCHeroModule::Init()
{
    return true;
}

bool NFCHeroModule::Shut()
{

    return true;
}

bool NFCHeroModule::Execute()
{

    return true;
}

bool NFCHeroModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogicClassModule );

    return true;
}

bool NFCHeroModule::BeforeShut()
{

    return true;
}

bool NFCHeroModule::AddHero( const NFGUID& self, const std::string& strID )
{
    return true;
}

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xID)
{
    return true;
}