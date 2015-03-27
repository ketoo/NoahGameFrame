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
    //位置呢
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
	//应记录玩家处于PVP筹备状态
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_FREE)
	{
		return false;
	}

	return true;
}

bool NFCPVPModule::StartPVPWar( const NFIDENTID& self )
{
	//应记录玩家处于PVP状态，或者说模块独占状态
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState != NLST_PVP)
	{
		return false;
	}

    m_pKernelModule->SetPropertyInt(self, "LogicState", (NFLogicStateType)NLST_PVP); // 进入PVP状态
    mxPVPList.Add(self);

    if (mxPVPList.Count() < PVPCell::PVPCell_Player_Count)
    {
        return true;
    }
    
    //record
    //switch

    //mxPVPList remove

    // TODO:发送次玩家的信息给其他玩家，发送其他玩家的信息给此玩家

	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFIDENTID& self )
{
	//还原玩家为自由状态
	const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	if (eLogicState == NLST_PVP)
	{
		m_pKernelModule->SetPropertyInt(self, "LogicState", NLST_FREE);
	}

	// 移除该玩家
    mxPVPList.Remove(self);

	return false;
}
