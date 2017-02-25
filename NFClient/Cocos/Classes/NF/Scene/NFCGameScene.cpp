// -------------------------------------------------------------------------
//    @FileName			:    NFCGameScene.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCGameScene
//
// -------------------------------------------------------------------------
#include "stdafx.h"
#include "Logic/NFCNetLogic.h"
#include "Logic/NFCPlayerLogic.h"
#include "Logic/NFCChatLogic.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFCGameScene.h"
#include "UI/NFCUIChatPanel.h"

NFCGameScene::NFCGameScene()
{
}

NFCGameScene::~NFCGameScene()
{
}

bool NFCGameScene::initLayout()
{	
	auto map = TMXTiledMap::create("Scene/NFMap.tmx");
	addChild(map);
	map->setPosition(map->getContentSize()*-0.5);
	
    g_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCGameScene::OnObjectClassEvent);

	g_pPlayerLogic->AddEventCallBack(E_PlayerEvent_PlayerMove, this, &NFCGameScene::OnPlayerMoveEvent);
	g_pChatLogic->AddEventCallBack(E_ChatEvent_ChatWorld, this, &NFCGameScene::OnPlayerChatEvent);

	scheduleUpdate();

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	NFCUIChatPanel::showPanel();

	return true;
}

void NFCGameScene::initData(void *pData)
{
}

void NFCGameScene::update(float dt)
{
	if (m_Players.Count() == 0)
		return;

	Vec2 size = Director::getInstance()->getWinSize();
	const NFGUID &self = g_pPlayerLogic->GetRoleGuid();
	Sprite *pPlayer = m_Players.GetElement(self);
	if (pPlayer)
	{
		Vec2 pos = pPlayer->getPosition();
		this->setPosition((size*0.5 - pos));
	}
}

bool NFCGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 toPos = this->convertTouchToNodeSpace(touch);
	g_pPlayerLogic->RequireMove(NFVector3(toPos.x, toPos.y, 0));
	return true;
}

void NFCGameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	Vec2 toPos = this->convertTouchToNodeSpace(touch);
	g_pPlayerLogic->RequireMove(NFVector3(toPos.x, toPos.y, 0));
}

int NFCGameScene::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
			Sprite *pPlayer = Sprite::create("grossini.png");
			pPlayer->setAnchorPoint(Vec2(0.5, 0));
			m_Players.AddElement(self, pPlayer);
			addChild(pPlayer);
			g_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Name(), this, &NFCGameScene::OnObjectPropertyEvent);
			update(0.0f);
        }
        else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
			m_Players.GetElement(self)->removeFromParent();
			m_Players.RemoveElement(self);
        }
    }

    return 0;
}

int NFCGameScene::OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	Sprite *pPlayer = m_Players.GetElement(self);
	auto pName = ui::Text::create(newVar.GetString(), "", 16);
	pName->setAnchorPoint(Vec2(0.5, 0));
	pName->setPosition(Vec2(pPlayer->getAnchorPointInPoints().x, pPlayer->getContentSize().height));
	pPlayer->addChild(pName);
	return 0;
}

int NFCGameScene::OnPlayerMoveEvent(const int nEventID, const NFDataList& varDataList)
{
	const NFGUID &self = varDataList.Object(0);
	Sprite *pPlayer = m_Players.GetElement(self);
	const NFVector3 &toPos = varDataList.Vector3(2);
	
	pPlayer->stopAllActions();
	pPlayer->runAction(CCMoveTo::create(1.0f, Vec2(toPos.X(), toPos.Y())));

	return 0;
}

int NFCGameScene::OnPlayerChatEvent(const int nEventID, const NFDataList& varDataList)
{
	const NFGUID &self = varDataList.Object(0);
	Sprite *pPlayer = m_Players.GetElement(self);

	auto pChat = ui::Text::create(varDataList.String(3), "", 16);
	pChat->setAnchorPoint(Vec2(0.5, 0));
	pChat->setPosition(Vec2(pPlayer->getAnchorPointInPoints().x, pPlayer->getContentSize().height+ pChat->getContentSize().height));
	pPlayer->addChild(pChat);
	pChat->runAction(Sequence::create(FadeIn::create(0.5), DelayTime::create(5.0f), FadeIn::create(0.5), RemoveSelf::create(), NULL));
	pChat->runAction(MoveBy::create(0.5, Vec2(0, pChat->getContentSize().height)));

	return 0;
}