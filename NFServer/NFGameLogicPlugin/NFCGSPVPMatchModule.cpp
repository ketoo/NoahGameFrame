// -------------------------------------------------------------------------
//    @FileName         :    NFCGSPVPMatchModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCGSPVPMatchModule
//
// -------------------------------------------------------------------------

#include "NFCGSPVPMatchModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCGSPVPMatchModule::Init()
{
    return true;
}


bool NFCGSPVPMatchModule::Shut()
{
    return true;
}

bool NFCGSPVPMatchModule::Execute()
{
    
    return true;
}

bool NFCGSPVPMatchModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pGSSwitchServerModule = pPluginManager->FindModule<NFIGSSwichServerModule>();
	
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_PVPAPPLYMACTCH, this, &NFCGSPVPMatchModule::OnReqPVPMatchProcess)) { return false; }

	if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_PVPAPPLYMACTCH, this, &NFCGSPVPMatchModule::OnAckPVPMatchProcess)) { return false; }
	if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATEPVPECTYPE, this, &NFCGSPVPMatchModule::OnReqCreatePVPEctyProcess)) { return false; }
	if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_CREATEPVPECTYPE, this, &NFCGSPVPMatchModule::OnAckCreatePVPEctyProcess)) { return false; }

    return true;
}


void NFCGSPVPMatchModule::OnReqPVPMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqPVPApplyMatch);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.self_id())))
	{
		return;
	}
 	NFGUID xTeamID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::TeamID());
 	*xMsg.mutable_team_id() = NFINetModule::NFToPB(xTeamID);
	m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(nPlayerID.GetData(), NFMsg::EGMI_REQ_PVPAPPLYMACTCH, xMsg);
}

void NFCGSPVPMatchModule::OnAckPVPMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::AckPVPApplyMatch);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.self_id())))
	{
		return;
	}

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_PVPAPPLYMACTCH, xMsg, nPlayerID);
}

void NFCGSPVPMatchModule::OnReqCreatePVPEctyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCreatePVPEctype);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.self_id())))
	{
		return;
	}

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_CREATEPVPECTYPE, xMsg, nPlayerID);
}

void NFCGSPVPMatchModule::OnAckCreatePVPEctyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::AckCreatePVPEctype);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.self_id())))
	{
		return;
	}

	if (xMsg.has_xroominfo())
	{
		int nServerID = xMsg.xroominfo().serverid();
		int nSceneID = xMsg.xroominfo().sceneid();
		int nGroupID = xMsg.xroominfo().groupid();

		const NFMsg::PVPRoomInfo& xRoomInfo = *xMsg.mutable_xroominfo();
		for (int i = 0; i < xRoomInfo.xblueplayer_size(); i++)
		{
			NFMsg::Ident xPBID = xRoomInfo.xblueplayer(i);
			NFGUID xPlayerID = NFINetModule::PBToNF(xPBID);
			if (m_pKernelModule->GetObject(xPlayerID))
			{
				m_pGSSwitchServerModule->ChangeServer(nPlayerID, nServerID, nSceneID, nGroupID);
			}
		}

		for (int i = 0; i < xRoomInfo.xredplayer_size(); i++)
		{
			NFMsg::Ident xPBID = xRoomInfo.xredplayer(i);
			NFGUID xPlayerID = NFINetModule::PBToNF(xPBID);
			if (m_pKernelModule->GetObject(xPlayerID))
			{
				m_pGSSwitchServerModule->ChangeServer(nPlayerID, nServerID, nSceneID, nGroupID);
			}
		}
	}
}
