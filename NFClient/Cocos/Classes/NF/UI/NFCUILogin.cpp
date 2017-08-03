// -------------------------------------------------------------------------
//    @FileName			:    NFCUILogin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUILogin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCUILogin.h"
#include "Logic/NFCLoginLogic.h"
#include "Logic/NFCNetLogic.h"

#include "NFCUISelectServer.h"

NFCUILogin::NFCUILogin()
{
}

NFCUILogin::~NFCUILogin()
{
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_LoginSuccess, this);
}

bool NFCUILogin::initLayout()
{	
	if(!NFCUIDialog::initLayout())
		return false;
		
	GetObjectByName(&m_pLoginButton, "login_Button");
	GetObjectByName(&m_pUserName, "name_TextField");
	GetObjectByName(&m_pUserPWD, "password_TextField");

	m_pUserName->setString("test1");
	m_pUserPWD->setString("123456");

	m_pLoginButton->addClickEventListener(CC_CALLBACK_1(NFCUILogin::onLoginTouch, this));
		
	g_pNetLogic->ConnectServer("104.160.35.67", 14001);
	//g_pNetLogic->ConnectServer("127.0.0.1", 14001);
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_LoginSuccess, this, &NFCUILogin::OnLoginEvent);
	return true;
}

void NFCUILogin::initData(void *customData)
{
}


void NFCUILogin::onLoginTouch(Ref *sender)
{
	g_pLoginLogic->LoginPB(m_pUserName->getString(), m_pUserPWD->getString(), "");
}

int NFCUILogin::OnLoginEvent(const int nEventID, const NFDataList& varDataList)
{
	NFCUISelectServer::showUI();
	return 0;
}
