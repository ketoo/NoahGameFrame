// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.cpp
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
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_FREE)
	{
		return false;
	}

	return true;
}

bool NFCPVPModule::StartPVPWar( const NFIDENTID& self )
{
	//Ӧ��¼��Ҵ���PVP״̬������˵ģ���ռ״̬
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_PVP)
	{
		return false;
	}

	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFIDENTID& self )
{
	//��ԭ���Ϊ����״̬
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState == NLST_PVP)
	{
		m_pKernelModule->SetPropertyInt(self, "LogicState", NLST_FREE);
	}

	//�������������飬�����¼��ص�

	return false;
}
