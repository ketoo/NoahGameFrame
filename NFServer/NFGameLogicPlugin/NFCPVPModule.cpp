// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#include "NFCPVPModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"

bool NFCPVPModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pGameServerNet_ServerModule );

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_PVP, this, &NFCPVPModule::OnClientJoinPVP)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_EXIT_PVP, this, &NFCPVPModule::OnClientExitPVP)){ return false; }

    return true;
}


bool NFCPVPModule::Shut()
{
    return true;
}

bool NFCPVPModule::Execute()
{
    //位置呢
    return true;
}

bool NFCPVPModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );

    return true;
}

bool NFCPVPModule::MatchPVPObject( const NFGUID& self )
{
	//应记录玩家处于PVP筹备状态
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState != NLST_FREE)
	//{
	//	return false;
	//}

	//随机选择战斗对象，并把对象数据下发

	return true;
}

bool NFCPVPModule::StartPVPWar( const NFGUID& self )
{
	//应记录玩家处于PVP状态，或者说模块独占状态
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState != NLST_PVP)
	//{
	//	return false;
	//}

 //   m_pKernelModule->SetPropertyInt(self, "LogicState", (NFLogicStateType)NLST_PVP); // 进入PVP状态
 //   mxPVPList.Add(self);

 //   if (mxPVPList.Count() < PVPCell::PVPCell_Player_Count)
 //   {
 //       return true;
 //   }
    
    //record
    //switch

    //mxPVPList remove

    // TODO:发送次玩家的信息给其他玩家，发送其他玩家的信息给此玩家

	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFGUID& self )
{
	//还原玩家为自由状态
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState == NLST_PVP)
	//{
	//	m_pKernelModule->SetPropertyInt(self, "LogicState", NLST_FREE);
	//}

	//// 移除该玩家
 //   mxPVPList.Remove(self);

	return false;
}

void NFCPVPModule::OnClientJoinPVP(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckJoinActivity)

	switch (xMsg.activity_type())
	{
		case NFMsg::ReqAckJoinActivity_EGameActivityType_EGAT_PVP:
			StartPVPWar(nPlayerID);
			break;
		default:
			break;
	}
}
void NFCPVPModule::OnClientExitPVP(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::reqex)

}
