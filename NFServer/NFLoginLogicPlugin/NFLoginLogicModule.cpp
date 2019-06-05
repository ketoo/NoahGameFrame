/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFLoginLogicPlugin.h"
#include "NFLoginLogicModule.h"


bool NFLoginLogicModule::Init()
{
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFLoginLogicModule::Shut()
{
    return true;
}

void NFLoginLogicModule::OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAccountLogin xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		if (pNetObject->GetConnectKeyState() == 0)
		{
			NFMsg::AckEventResult xAckMsg;

			switch (xMsg.loginmode())
			{
			case NFMsg::ELM_AUTO_REGISTER_LOGIN: // auto register when login
				if (m_pAccountRedisModule->AddAccount(xMsg.account(), xMsg.password()))
				{
					break;
				}
				// goto case NFMsg::ELM_LOGIN

			case NFMsg::ELM_LOGIN: // login
				if (!m_pAccountRedisModule->VerifyAccount(xMsg.account(), xMsg.password()))
				{
					std::ostringstream strLog;
					strLog << "Check password failed, Account = " << xMsg.account() << " Password = " << xMsg.password();
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSockIndex), strLog, __FUNCTION__, __LINE__);

					xAckMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);
					m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xAckMsg, nSockIndex);
					return;
				}
				break;

			case NFMsg::ELM_REGISTER: // register
				if (!m_pAccountRedisModule->AddAccount(xMsg.account(), xMsg.password()))
				{
					std::ostringstream strLog;
					strLog << "Create account failed, Account = " << xMsg.account() << " Password = " << xMsg.password();
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSockIndex), strLog, __FUNCTION__, __LINE__);

					xAckMsg.set_event_code(NFMsg::EGEC_ACCOUNT_EXIST);
					m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xAckMsg, nSockIndex);
					return;
				}
				break;

			default:
				break;
			}

			pNetObject->SetConnectKeyState(1);
			pNetObject->SetAccount(xMsg.account());

			xAckMsg.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);
			m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xAckMsg, nSockIndex);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "Login successed :", xMsg.account().c_str());
		}
	}
}

bool NFLoginLogicModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_LOGIN);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGIN, this, &NFLoginLogicModule::OnLoginProcess);

    return true;
}

bool NFLoginLogicModule::Execute()
{


    return true;
}


bool NFLoginLogicModule::AfterInit()
{
    return true;
}
