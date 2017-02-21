// -------------------------------------------------------------------------
//    @FileName			:    NFCUISelectServer.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUISelectServer
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCUISelectServer.h"
#include "Logic/NFCLoginLogic.h"
#include "Logic/NFCNetLogic.h"

#include "NFCUISelectRole.h"

NFCUISelectServer::NFCUISelectServer()
{
}

NFCUISelectServer::~NFCUISelectServer()
{
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_WorldList, this);
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_ServerList, this);
}

bool NFCUISelectServer::initLayout()
{	
	if(!NFCUIDialog::initLayout())
		return false;

	m_pServerList = ui::ListView::create();
	m_pServerList->setSizeType(ui::SizeType::PERCENT);
	m_pServerList->setSizePercent(Vec2(0.1f, 0.25f));
	m_pServerList->setPositionType(ui::PositionType::PERCENT);
	m_pServerList->setPositionPercent(Vec2(0.5f, 0.5f));
	m_pServerList->setBackGroundColorType(ui::LayoutBackGroundColorType::SOLID);
	m_pServerList->setBackGroundColor(Color3B(150, 200, 255));
	m_pServerList->setAnchorPoint(Vec2(0.5, 0.5));
	m_pContent->addChild(m_pServerList);

	auto button = ui::Button::create();
	button->loadTextureNormal("Default/Button_Normal.png",ui::TextureResType::LOCAL);
	button->setScale9Enabled(true);
	button->setSizePercent(Vec2(1.0f, 0.2f));
	button->setSizeType(ui::SizeType::PERCENT);

	m_pServerList->setItemModel(button);
	
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_WorldList, this, &NFCUISelectServer::OnWorldListEvent);
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_ServerList, this, &NFCUISelectServer::OnServerListEvent);
	return true;
}

void NFCUISelectServer::initData(void *customData)
{
	g_pLoginLogic->RequireWorldList();
	m_pServerList->removeAllItems();
}

int NFCUISelectServer::OnWorldListEvent(const int nEventID, const NFDataList& varDataList)
{
	auto worldList = g_pLoginLogic->GetWorldList();
	for(size_t i = 0; i < worldList.size(); i++)
	{
		const NFMsg::ServerInfo &kServerInfo =  worldList[i];
		m_pServerList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pServerList->getItem(m_pServerList->getItems().size()-1);
		pButton->addClickEventListener(CC_CALLBACK_1(NFCUISelectServer::OnWorldSelected, this));
		pButton->setTitleText(kServerInfo.name());
		pButton->setTitleColor(Color3B(0, 0, 0));
	}

	return 0;
}

int NFCUISelectServer::OnServerListEvent(const int nEventID, const NFDataList& varDataList)
{
	auto worldList = g_pLoginLogic->GetServerList();
	for(size_t i = 0; i < worldList.size(); i++)
	{
		const NFMsg::ServerInfo &kServerInfo =  worldList[i];
		m_pServerList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pServerList->getItem(m_pServerList->getItems().size()-1);
		pButton->addClickEventListener(CC_CALLBACK_1(NFCUISelectServer::OnServerSelected, this));
		pButton->setTitleText(kServerInfo.name());
		pButton->setTitleColor(Color3B(0, 0, 0));
	}

	return 0;
}

void NFCUISelectServer::OnWorldSelected(Ref* sender)
{
	auto worldList = g_pLoginLogic->GetWorldList();
	int nIndex = m_pServerList->getIndex((ui::Widget*)sender);
	g_pLoginLogic->RequireConnectWorld(worldList[nIndex].server_id());
	m_pServerList->removeAllItems();
}

void NFCUISelectServer::OnServerSelected(Ref* sender)
{
	auto serverList = g_pLoginLogic->GetServerList();
	int nIndex = m_pServerList->getIndex((ui::Widget*)sender);
	g_pLoginLogic->RequireSelectServer(serverList[nIndex].server_id());
	NFCUISelectRole::showUI(true);
}