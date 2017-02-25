// -------------------------------------------------------------------------
//    @FileName			:    NFCUIChatPanel.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIChatPanel
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCUIChatPanel.h"
#include "Logic/NFCChatLogic.h"
#include "Logic/NFCNetLogic.h"

#include "NFCUISelectServer.h"

NFCUIChatPanel::NFCUIChatPanel()
{
}

NFCUIChatPanel::~NFCUIChatPanel()
{
}

bool NFCUIChatPanel::initLayout()
{	
	if(!NFCUIDialog::initLayout())
		return false;
		
	GetObjectByName(&m_pSend, "m_pSend");
	GetObjectByName(&m_pInputText, "m_pInputText");

	m_pSend->addClickEventListener(CC_CALLBACK_1(NFCUIChatPanel::onLoginTouch, this));
	return true;
}

void NFCUIChatPanel::initData(void *customData)
{
}


void NFCUIChatPanel::onLoginTouch(Ref *sender)
{
	g_pChatLogic->RequireChatWorld(m_pInputText->getString());
	m_pInputText->setString("");
}
