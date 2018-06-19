// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFLoginLogicPlugin.h"
#include "NFCLoginLogicModule.h"


bool NFCLoginLogicModule::Init()
{
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFCLoginLogicModule::Shut()
{
    return true;
}

void NFCLoginLogicModule::OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

bool NFCLoginLogicModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_LOGIN);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGIN, this, &NFCLoginLogicModule::OnLoginProcess);

    return true;
}

bool NFCLoginLogicModule::Execute()
{


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{
    return true;
}
