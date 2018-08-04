/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


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
    m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
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
    if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_REQSWICHSERVER, this, &NFCSwitchGameServerModule::OnReqSwitchServer)) { return false; }
    if (!m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACKSWICHSERVER, this, &NFCSwitchGameServerModule::OnAckSwitchServer)) { return false; }

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

    m_pSceneAOIModule->RequestEnterScene(pObject->Self(), nSceneID, nGroup, 0, NFDataList());
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

bool NFCSwitchGameServerModule::SwitchServer(const NFGUID &self, const int nServer, const int nSceneID, const int nGroup)
{
    //lock the user
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

	return false;
}
