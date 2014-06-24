// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerNet_ServerModule.h"
#include "NFComm\NFPluginModule\NFILogicModule.h"

bool NFCGameServerNet_ServerModule::Init()
{
    mnRoleHallContainer = -3;
    mnProxyContainer = -4;
    mstrConfigIdent = "GameServer";
    



    return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>(pPluginManager->FindModule("NFCSceneProcessModule"));
    m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>(pPluginManager->FindModule("NFCGameLogicModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pGameLogicModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);
//     m_pKernelModule->LogInfo(" -1 ConnectContainer ");
//     m_pKernelModule->LogInfo(" -2 WantToConnectContainer ");

//     m_pKernelModule->ResgisterCommonClassEvent(OnClassCommonEvent);
//     m_pKernelModule->ResgisterCommonPropertyEvent(OnPropertyCommonEvent);
//     m_pKernelModule->ResgisterCommonRecordEvent(OnRecordCommonEvent);
// 
//     m_pEventProcessModule->AddClassCallBack("Player", OnObjectClassEvent);

    //  m_pEventProcessModule->AddClassCallBack("AttackNPC", OnClassObjectEvent);
    //  m_pEventProcessModule->AddClassCallBack("Pet", OnClassObjectEvent);

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ServerModule::OnRecivePack, &NFCGameServerNet_ServerModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
	if (nRet <= 0)
	{
		assert(0);
	}


    return true;
}

bool NFCGameServerNet_ServerModule::Shut()
{

    return true;
}

bool NFCGameServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCGameServerNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    // TOCHECK
    //NFIActorMessage xActorMsg;

    //xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG;
    //xActorMsg.nSubMsgID = msg.GetMsgHead()->GetMsgID();
    //xActorMsg.data.assign(msg.GetData(), msg.GetDataLen());

    ////根据用户基础信息得到目标actor地址,看自己想怎么管理和优化
    //pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());
    //不上actor的情况
    switch (msg.GetMsgHead()->GetMsgID())
    {
    case NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME:
        OnClienEnterGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME:
        OnClienLeaveGameProcess(msg);
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST:
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE:
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_DELETE_ROLE:
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_RECOVER_ROLE:
        break;
    default:
        break;
    }

    return true;
}

int NFCGameServerNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }

	return true;
}

void NFCGameServerNet_ServerModule::OnClientDisconnect( const int nAddress )
{

}

void NFCGameServerNet_ServerModule::OnClientConnected( const int nAddress )
{
	NFCValueList varList;
	m_pKernelModule->GetObjectByProperty(mnProxyContainer, "FD", NFCValueList() << nAddress, varList);
	if (varList.GetCount() > 0)
	{
		m_pKernelModule->DestroyObject(varList.ObjectVal(0));		
	}
}

void NFCGameServerNet_ServerModule::OnClienEnterGameProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;	
    NFMsg::ReqEnterGameServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    if (0 == nPlayerID)
    {
        return;
    }

    nPlayerID = xMsg.player_id();
    //server_id
    //gate_id

    int nSceneID = 1;
    NFCValueList valArg;
    valArg << "ProxyID";
    valArg << xMsg.gate_id();
    NFIObject* pObject = m_pKernelModule->CreateObject( nPlayerID, nSceneID, 0, "Player", "", valArg );
    if ( NULL == pObject )
    {
        return;
    }

    m_pEventProcessModule->DoEvent(pObject->Self(), "Player", CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCValueList() );
}

void NFCGameServerNet_ServerModule::OnClienLeaveGameProcess( const NFIPacket& msg )
{
    int64_t nPlayerID = 0;	
    NFMsg::ReqLeaveGameServer xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return;
    }

    if (0 == nPlayerID)
    {
        return;
    }

    m_pKernelModule->DestroyObject(nPlayerID);
}

int NFCGameServerNet_ServerModule::OnPropertyEnter( const NFIDENTID& self, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnRecordEnter( const NFIDENTID& self, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnObjectListEnter( const NFIValueList& self, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnObjectListLeave( const NFIValueList& self, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnGroupEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnContainerEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    return 0;
}

int NFCGameServerNet_ServerModule::GetBroadCastObject( const NFIDENTID& self, const std::string& strPropertyName, const bool bTable, NFIValueList& valueObject )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnReturnEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnMoveEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnUseSkillResultEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnSwapSceneResultEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}

int NFCGameServerNet_ServerModule::OnChatResultEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}

void NFCGameServerNet_ServerModule::OnCreateRoleGameProcess( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnDeleteRoleGameProcess( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienSwapSceneProcess( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienUseSkill( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienMove( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienCommand( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienAcceptTask( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPushTask( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPushCustom( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienPickItem( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienChatProcess( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienUseItem( const NFIPacket& msg )
{

}
