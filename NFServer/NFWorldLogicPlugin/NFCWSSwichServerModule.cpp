// -------------------------------------------------------------------------
//    @FileName         :    NFCWSSwichServerModule.cpp
//    @Author               :    s.Guo
//    @Date                 :    2013-06-11
//    @Module               :    NFCWSSwichServerModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCWSSwichServerModule.h"

bool NFCWSSwichServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pWorlNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();


	return true;
}

bool NFCWSSwichServerModule::Shut()
{
    return true;
}

bool NFCWSSwichServerModule::Execute()
{
    
    return true;
}

bool NFCWSSwichServerModule::AfterInit()
{
	
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQSWICHSERVER, this, &NFCWSSwichServerModule::OnReqSwichServer)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACKSWICHSERVER, this, &NFCWSSwichServerModule::OnAckSwichServer)) { return false; }

    return true;
}

void NFCWSSwichServerModule::OnReqSwichServer( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
	nPlayerID = NFINetModule::PBToNF(xMsg.selfid());

	const NFGUID nClientID = NFINetModule::PBToNF(xMsg.client_id());
	const int nGateID = (int)xMsg.gate_serverid(); 
	const int nSceneID = (int)xMsg.sceneid();
	m_pWorlNet_ServerModule->SendMsgToGame((int)xMsg.target_serverid(), NFMsg::EGMI_REQSWICHSERVER, xMsg, nPlayerID);
}

void NFCWSSwichServerModule::OnAckSwichServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckSwitchServer);
	nPlayerID = NFINetModule::PBToNF(xMsg.selfid());

	m_pWorlNet_ServerModule->SendMsgToGame((int)xMsg.self_serverid(), NFMsg::EGMI_ACKSWICHSERVER, xMsg, nPlayerID);
}
