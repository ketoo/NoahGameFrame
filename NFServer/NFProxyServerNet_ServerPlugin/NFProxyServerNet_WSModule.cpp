/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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
#include "NFProxyServerNet_WSModule.h"
#include "NFProxyServerNet_ServerPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFProxyServerNet_WSModule::Init()
{
	m_pWSModule = pPluginManager->FindModule<NFIWSModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pProxyToWorldModule = pPluginManager->FindModule<NFIProxyServerToWorldModule>();
	m_pSecurityModule = pPluginManager->FindModule<NFISecurityModule>();

    return true;
}

bool NFProxyServerNet_WSModule::AfterInit()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (serverType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->GetAppID() == serverID)
            {
                const int port = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                const int cpuCount = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                //const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
                //const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());
                int wsPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::WSPort());

                //web server only run one instance in each server
                if (wsPort > 0)
                {
                    int nRet = m_pWSModule->Initialization(maxConnect, wsPort, cpuCount);
                    if (nRet < 0)
                    {
                        std::ostringstream strLog;
                        strLog << "Cannot init websocket server net, Port = " << wsPort;
                        m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
                        NFASSERT(nRet, "Cannot init websocket server net", __FILE__, __FUNCTION__);
                        exit(0);
                    }

	                m_pWSModule->AddEventCallBack(this, &NFProxyServerNet_WSModule::OnSocketClientEvent);
	                m_pWSModule->AddReceiveCallBack(this, &NFProxyServerNet_WSModule::OnWebSocketTestProcess);
                    break;
                }
                
            }
        }
    }

    return true;
}

bool NFProxyServerNet_WSModule::Shut()
{
    return true;
}

bool NFProxyServerNet_WSModule::Execute()
{
	return true;
}

void NFProxyServerNet_WSModule::OnWebSocketTestProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	m_pWSModule->SendMsgToAllClient(std::string(msg, len));
}

void NFProxyServerNet_WSModule::OnSocketClientEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_EOF Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(sockIndex);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_ERROR Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(sockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_TIMEOUT read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(sockIndex);
    }
    else  if (eEvent & NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
        OnClientConnected(sockIndex);
    }
}

void NFProxyServerNet_WSModule::OnClientDisconnect(const NFSOCK nAddress)
{
    NetObject* pNetObject = m_pWSModule->GetNet()->GetNetObject(nAddress);
    if (pNetObject)
    {
        int nGameID = pNetObject->GetGameID();
        if (nGameID > 0)
        {
            //when a net-object bind a account then tell that game-server
            if (!pNetObject->GetUserID().IsNull())
            {
                /*
                NFMsg::ReqLeaveGameServer xData;
				xData.set_arg(nGameID);

                NFMsg::MsgBase xMsg;

				//real user id
                *xMsg.mutable_player_id() = NFINetModule::NFToPB(pNetObject->GetUserID());

                if (!xData.SerializeToString(xMsg.mutable_msg_data()))
                {
                    return;
                }

                std::string msg;
                if (!xMsg.SerializeToString(&msg))
                {
                    return;
                }

				m_pNetClientModule->SendByServerIDWithOutHead(nGameID, NFMsg::EGameMsgID::REQ_LEAVE_GAME, msg);
                 */
  
            }
        }

        mxClientIdent.RemoveElement(pNetObject->GetClientID());
    }
}

void NFProxyServerNet_WSModule::OnClientConnected(const NFSOCK nAddress)
{
	//bind client'id with socket id
    NetObject* pNetObject = m_pWSModule->GetNet()->GetNetObject(nAddress);
    if (pNetObject && pNetObject->GetClientID().IsNull())
    {
		NFGUID xClientIdent = m_pKernelModule->CreateGUID();
        pNetObject->SetClientID(xClientIdent);
		mxClientIdent.AddElement(xClientIdent, NF_SHARE_PTR<NFSOCK>(new NFSOCK(nAddress)));


		// 1. create a tcp client to connect to the TCP service provided by proxy server.
		// 2. transfer the ws data come from websocket to the TCP service provided by proxy server.
		// 3.transfer the tcp data come from proxy server to the websocket service to send to clients.
    }
}
