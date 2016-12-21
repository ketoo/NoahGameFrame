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
	
    return true;
}


bool NFCPVPModule::Shut()
{
    return true;
}

bool NFCPVPModule::Execute()
{
    
    return true;
}

bool NFCPVPModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_PVP, this, &NFCPVPModule::OnClientJoinPVP)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_EXIT_PVP, this, &NFCPVPModule::OnClientExitPVP)) { return false; }

    return true;
}

bool NFCPVPModule::MatchPVPObject( const NFGUID& self )
{
	
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState != NLST_FREE)
	//{
	//	return false;
	//}

	

	return true;
}

bool NFCPVPModule::StartPVPWar( const NFGUID& self )
{
	
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState != NLST_PVP)
	//{
	//	return false;
	//}

 
 //   mxPVPList.Add(self);

 //   if (mxPVPList.Count() < PVPCell::PVPCell_Player_Count)
 //   {
 //       return true;
 //   }
    
    //record
    //switch

    //mxPVPList remove

    

	return true;
}

bool NFCPVPModule::ExitPVPWar( const NFGUID& self )
{
	
	//const NFLogicStateType eLogicState = (NFLogicStateType)m_pKernelModule->GetPropertyInt(self, "LogicState");
	//if (eLogicState == NLST_PVP)
	//{
	//	m_pKernelModule->SetPropertyInt(self, "LogicState", NLST_FREE);
	//}

	
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
