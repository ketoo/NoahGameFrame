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
	m_pWorlNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>("NFCWorldNet_ServerModule");
	
	assert(NULL != m_pWorlNet_ServerModule);

	if (!m_pWorlNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQSWICHSERVER, this,		&NFCWSSwichServerModule::OnReqSwichServer)){ return false; }
	if (!m_pWorlNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_ACKSWICHSERVER, this,		&NFCWSSwichServerModule::OnAckSwichServer)){ return false; }
	return true;
}

bool NFCWSSwichServerModule::Shut()
{
    return true;
}

bool NFCWSSwichServerModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCWSSwichServerModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
	assert( NULL != m_pLogModule );

    return true;
}

void NFCWSSwichServerModule::OnReqSwichServer( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
	if (nPlayerID != NFINetModule::PBToNF(xMsg.selfid()))
	{
		return;
	}

	const NFGUID nClientID = NFINetModule::PBToNF(xMsg.client_id());
	const int nGateID = xMsg.gate_serverid(); 
	const int nSceneID = xMsg.sceneid();
	m_pWorlNet_ServerModule->SendMsgToGame((int)xMsg.target_serverid(), NFMsg::EGMI_REQSWICHSERVER, xMsg, nPlayerID);
}

void NFCWSSwichServerModule::OnAckSwichServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::AckSwitchServer);
	if (nPlayerID != NFINetModule::PBToNF((xMsg.selfid())))
	{
		return;
	}

	m_pWorlNet_ServerModule->SendMsgToGame((int)xMsg.self_serverid(), NFMsg::EGMI_ACKSWICHSERVER, xMsg, nPlayerID);
}
