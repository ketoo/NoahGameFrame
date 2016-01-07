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

bool NFCPVPModule::Execute()
{
    //λ����
    return true;
}

bool NFCPVPModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );


    return true;
}

bool NFCPVPModule::MatchPVPObject( const NFGUID& self )
{
	//Ӧ��¼��Ҵ���PVP�ﱸ״̬
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_FREE)
	{
		return false;
	}

	//���ѡ��ս�����󣬲��Ѷ��������·�

	return true;
}

bool NFCPVPModule::StartPVPWar( const NFGUID& self )
{
	//Ӧ��¼��Ҵ���PVP״̬������˵ģ���ռ״̬
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_PVP)
	{
		return false;
	}

    m_pKernelModule->SetPropertyInt(self, "LogicState", (NFLogicStateType)NLST_PVP); // ����PVP״̬
    mxPVPList.Add(self);

    if (mxPVPList.Count() < PVPCell::PVPCell_Player_Count)
    {
        return true;
    }
    
    //record
    //switch

    //mxPVPList remove

    // TODO:���ʹ���ҵ���Ϣ��������ң�����������ҵ���Ϣ�������

	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFGUID& self )
{
	//��ԭ���Ϊ����״̬
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState == NLST_PVP)
	{
		m_pKernelModule->SetPropertyInt(self, "LogicState", NLST_FREE);
	}

	// �Ƴ������
    mxPVPList.Remove(self);

	return false;
}
