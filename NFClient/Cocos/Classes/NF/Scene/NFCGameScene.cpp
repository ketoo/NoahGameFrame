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
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFCGameScene.h"

NFCGameScene::NFCGameScene()
{
}

NFCGameScene::~NFCGameScene()
{
}

bool NFCGameScene::initLayout()
{	
	//CCSprite *pBackGround = CCSprite::create("Scene/bbg_fine_ship.jpg");
	//pBackGround->setAnchorPoint(ccp(0,0));
	//pBackGround->setScale(1.2);
	//addChild(pBackGround);
	
    g_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCGameScene::OnObjectClassEvent);

	return true;
}

void NFCGameScene::initData(void *pData)
{
}

int NFCGameScene::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
			Sprite *pPlayer = Sprite::create("grossini.png");
			m_Players.AddElement(self, pPlayer);
			addChild(pPlayer);
        }
        else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
			m_Players.GetElement(self)->removeFromParent();
			m_Players.RemoveElement(self);
        }
    }

    return 0;
}