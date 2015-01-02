// -------------------------------------------------------------------------
//    @FileName      :    NFCPVPModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#include "NFCPVPModule.h"

bool NFCPVPModule::Init()
{


    return true;
}


bool NFCPVPModule::Shut()
{
    return true;
}

bool NFCPVPModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //λ����
    return true;
}

bool NFCPVPModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );


    return true;
}

bool NFCPVPModule::MatchPVPObject( const NFIDENTID& self )
{
	//Ӧ��¼��Ҵ���PVP�ﱸ״̬
	return true;
}

bool NFCPVPModule::StartPVPWar( const NFIDENTID& self )
{
	//Ӧ��¼��Ҵ���PVP״̬
	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFIDENTID& self )
{
	//��ԭ���Ϊ����״̬
	return true;
}
