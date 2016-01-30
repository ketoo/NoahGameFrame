// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatModule
//
// -------------------------------------------------------------------------

#include "NFWorldChatPlugin.h"
#include "NFCWorldChatModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCWorldChatModule::Init()
{
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>("NFCWorldNet_ServerModule");

	assert(NULL != m_pWorldNet_ServerModule);

	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCWorldChatModule::OnChatProcess)){ return false; }

    return true;
}

bool NFCWorldChatModule::Shut()
{
    return true;
}

bool NFCWorldChatModule::Execute()
{


    return true;
}

bool NFCWorldChatModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");

	assert(NULL != m_pKernelModule);

    return true;
}


void NFCWorldChatModule::OnChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat)
		switch (xMsg.chat_type())
	{
		case NFMsg::ReqAckPlayerChat::EGCT_WORLD:
			{
				//SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
			}
			break;
		default:
			break;;
	}
}

