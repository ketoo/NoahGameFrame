// -------------------------------------------------------------------------
//    @FileName         :    NFCGSSwichServerModule.cpp
//    @Author               :    s.Guo
//    @Date                 :    2013-06-11
//    @Module               :    NFCGSSwichServerModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCGSSwichServerModule.h"

bool NFCGSSwichServerModule::Init()
{
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>("NFCGameServerToWorldModule");
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
	
	assert(NULL != m_pGameServerToWorldModule);
	assert(NULL != m_pGameServerNet_ServerModule);

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQSWICHSERVER, this, &NFCGSSwichServerModule::OnClientReqSwichServer)) { return false; }
	if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_REQSWICHSERVER, this, &NFCGSSwichServerModule::OnReqSwichServer)) { return false; }
	if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACKSWICHSERVER, this,		&NFCGSSwichServerModule::OnAckSwichServer)){ return false; }
	return true;
}

bool NFCGSSwichServerModule::Shut()
{
    return true;
}

bool NFCGSSwichServerModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCGSSwichServerModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>("NFCLevelModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>("NFCHeroModule");
    
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
	assert( NULL != m_pLogModule );
    assert(NULL != m_pLevelModule);
    assert(NULL != m_pPackModule);
    assert(NULL != m_pHeroModule);

    return true;
}

bool NFCGSSwichServerModule::ChangeServer(const NFGUID& self, const int nServer, const int nSceneID)
{
	NFMsg::ReqSwitchServer xMsg;

	xMsg.set_sceneid(nSceneID);
	*xMsg.mutable_selfid() = NFINetModule::NFToPB(self);
	xMsg.set_self_serverid(pPluginManager->AppID());
	xMsg.set_target_serverid(nServer);

	int nGate = 0;
	NFGUID xClient;
	if (!m_pGameServerNet_ServerModule->GetGateInfo(self, nGate, xClient))
	{
		return false;
	}
	*xMsg.mutable_client_id() = NFINetModule::NFToPB(xClient);
	xMsg.set_gate_serverid(nGate);

	m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(self.ToString(), NFMsg::EGMI_REQSWICHSERVER, xMsg);

	return true;
}

void NFCGSSwichServerModule::OnClientReqSwichServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.selfid()))
	{
		return;
	}

	if (xMsg.target_serverid() == pPluginManager->AppID())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Target server is this server", xMsg.target_serverid(), __FUNCTION__, __LINE__);
		return;
	}

	ChangeServer(nPlayerID, xMsg.target_serverid(), xMsg.sceneid());
}

void NFCGSSwichServerModule::OnReqSwichServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.selfid()))
	{
		return;
	}

	if (xMsg.target_serverid() != pPluginManager->AppID())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Target server is not this server", xMsg.target_serverid(), __FUNCTION__, __LINE__);
		return;
	}

	const NFGUID nClientID = NFINetModule::PBToNF(xMsg.client_id());
	const int nGateID = xMsg.gate_serverid(); 
	const int nSceneID = xMsg.sceneid();
	m_pGameServerNet_ServerModule->ProcessSwitchToGame(nPlayerID, nClientID, nGateID, nSceneID);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_REQSWICHSERVER, xMsg, nPlayerID);
	m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(nPlayerID.ToString(), NFMsg::EGMI_ACKSWICHSERVER, xMsg);
}

void NFCGSSwichServerModule::OnAckSwichServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::AckSwitchServer);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.selfid())))
	{
		return;
	}

	m_pGameServerNet_ServerModule->ProcessSwitchOffGame(nPlayerID);
}
