/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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


#include "NFCGameServerToDBModule.h"
#include "NFGameServerNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCGameServerToDBModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFCGameServerToDBModule::Shut()
{

	return true;
}


bool NFCGameServerToDBModule::Execute()
{
	return true;
}

bool NFCGameServerToDBModule::AfterInit()
{
	//m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFCGameServerToDBModule::TransPBToProxy);
	//m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFCGameServerToDBModule::OnSocketWSEvent);
	
	m_pNetClientModule->ExpandBufferSize();

	return true;
}

void NFCGameServerToDBModule::OnSocketWSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connected success", __FUNCTION__, __LINE__);
	}
}

void NFCGameServerToDBModule::TransPBToProxy(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, msg, nLen);

	return;
}

void NFCGameServerToDBModule::TransmitToDB(const int nHashKey, const int nMsgID, const google::protobuf::Message& xData)
{
	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_DB, nHashKey, nMsgID, xData);
}