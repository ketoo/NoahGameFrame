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

    return true;
}

bool NFCLoginLogicModule::Shut()
{
    return true;
}

void NFCLoginLogicModule::OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFINetModule* pNetModule = m_pLoginNet_ServerModule->GetNetModule();

    NFGUID nPlayerID;
	NFMsg::ReqAccountLogin xMsg;
	if (!pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NetObject* pNetObject = pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		if (pNetObject->GetConnectKeyState() == 0)
		{
			//int nState = m_pLoginLogicModule->OnLoginProcess(pNetObject->GetClientID(), xMsg.account(), xMsg.password());
            int nState = 0;
			if (0 != nState)
			{
				std::ostringstream strLog;
				strLog << "Check password failed, Account = " << xMsg.account() << " Password = " << xMsg.password();
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSockIndex), strLog, __FUNCTION__, __LINE__);

				NFMsg::AckEventResult xMsg;
				xMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);

				pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xMsg, nSockIndex);
				return;
			}

			pNetObject->SetConnectKeyState(1);
			pNetObject->SetAccount(xMsg.account());

			NFMsg::AckEventResult xData;
			xData.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);

			pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xData, nSockIndex);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "Login successed :", xMsg.account().c_str());
		}
	}
}

bool NFCLoginLogicModule::ReadyExecute()
{
    NFINetModule* pNetModule = m_pLoginNet_ServerModule->GetNetModule();
    pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_LOGIN);
	pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGIN, this, &NFCLoginLogicModule::OnLoginProcess);

    return true;
}

bool NFCLoginLogicModule::Execute()
{


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{
    m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    return true;
}
