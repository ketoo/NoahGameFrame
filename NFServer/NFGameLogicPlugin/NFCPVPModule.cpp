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
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

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

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_PVP, this, &NFCPVPModule::OnClientJoinPVP)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_EXIT_PVP, this, &NFCPVPModule::OnClientExitPVP)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_OPPNENT, this, &NFCPVPModule::OnSearchOppnent)) { return false; }
	
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

void NFCPVPModule::OnSearchOppnent(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSearchOppnent);
	//find a tile map and swap scene

	int nSceneID = 1;
	std::string strTileData;
	if (m_pPlayerRedisModule->LoadPlayerTileRandom(nSceneID, strTileData))
	{
		NFMsg::AckMiningTitle xTileData;
		if (xTileData.ParseFromString(strTileData))
		{
			//tell client u should load resources
			NFMsg::AckSearchOppnent xAckData;
			xAckData.set_scene_id(nSceneID);
			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_OPPNENT, xAckData, nSockIndex);

			//tell client u shoud adjust tile
			m_pNetModule->SendMsgPB(NFMsg::EGEC_ACK_MINING_TITLE, xTileData, nSockIndex);
		}
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "ERROR TO FIND A OPPNENT!", "",  __FUNCTION__, __LINE__);
}
