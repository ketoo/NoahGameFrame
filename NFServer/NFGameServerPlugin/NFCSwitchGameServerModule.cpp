//
// Created by LUSHENG HUANG on 30/08/17.
//

#include "NFCSwitchGameServerModule.h"

bool NFCSwitchGameServerModule::Init()
{
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
    m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
    m_pSceneModule = pPluginManager->FindModule<NFISceneAOIModule>();
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    return true;
}

bool NFCSwitchGameServerModule::Shut()
{
    return true;
}

bool NFCSwitchGameServerModule::Execute()
{

    return true;
}

bool NFCSwitchGameServerModule::AfterInit()
{
    if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_REQSWICHSERVER, this, &NFCSwitchGameServerModule::OnReqSwichServer)) { return false; }
    if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACKSWICHSERVER, this, &NFCSwitchGameServerModule::OnAckSwichServer)) { return false; }

    return true;
}

void NFCSwitchGameServerModule::OnReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqSwitchServer);
    if (nPlayerID != NFINetModule::PBToNF(xMsg.selfid()))
    {
        return;
    }

    if (xMsg.target_serverid() != pPluginManager->GetAppID())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Target server is not this server", xMsg.target_serverid(), __FUNCTION__, __LINE__);
        return;
    }

    const NFGUID nClientID = NFINetModule::PBToNF(xMsg.client_id());
    const int nGateID = (int)xMsg.gate_serverid();
    const int nSceneID = (int)xMsg.sceneid();
    const int nGroup = (int)xMsg.groupid();

    //if (!AddPlayerGateInfo(nPlayerID, nClientID, nGateID))
    //   {
    //       return;
    //   }

    NFDataList var;
    var.AddString(NFrame::Player::GateID());
    var.AddInt(nGateID);

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nPlayerID, nSceneID, 0, NFrame::Player::ThisName(), "", var);
    if (NULL == pObject.get())
    {
        //mRoleBaseData
        //mRoleFDData
        return;
    }

    pObject->SetPropertyInt(NFrame::Player::GateID(), nGateID);
    pObject->SetPropertyInt(NFrame::Player::GameID(), pPluginManager->GetAppID());

    m_pKernelModule->DoEvent(pObject->Self(), NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFDataList());

    m_pSceneModule->RequestEnterScene(pObject->Self(), nSceneID, nGroup, 0, NFDataList());
    //m_pEventModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);

    if (!m_pGameServerNet_ServerModule->AddPlayerGateInfo(nPlayerID, nClientID, nGateID))
    {
        m_pKernelModule->DestroyObject(nPlayerID);
        return ;
    }

    m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_REQSWICHSERVER, xMsg, nPlayerID);
    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nPlayerID.ToString(), NFMsg::EGMI_ACKSWICHSERVER, xMsg);
}

void NFCSwitchGameServerModule::OnAckSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::AckSwitchServer);
    if (nPlayerID != NFINetModule::PBToNF((xMsg.selfid())))
    {
        return;
    }

    m_pGameServerNet_ServerModule->RemovePlayerGateInfo(nPlayerID);
    m_pKernelModule->DestroyObject(nPlayerID);
}

void NFCSwitchGameServerModule::SwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    NFMsg::ReqSwitchServer xMsg;

    xMsg.set_sceneid(nSceneID);
    *xMsg.mutable_selfid() = NFINetModule::NFToPB(self);
    xMsg.set_self_serverid(pPluginManager->GetAppID());
    xMsg.set_target_serverid(nServer);
    xMsg.set_groupid(nGroup);

    int nGate = 0;
    NFGUID xClient;

    NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo> pGateInfo = m_pGameServerNet_ServerModule->GetPlayerGateInfo(self);
    if (!pGateInfo)
    {
        return false;
    }

    nGate = pGateInfo->nGateID;
    xClient = pGateInfo->xClientID;
    *xMsg.mutable_client_id() = NFINetModule::NFToPB(xClient);
    xMsg.set_gate_serverid(nGate);

    m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, self.ToString(), NFMsg::EGMI_REQSWICHSERVER, xMsg);
}
