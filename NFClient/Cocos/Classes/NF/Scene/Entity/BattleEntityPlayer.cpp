#include "stdafx.h"

#include "BattleEntityPlayer.h"
#include "EntityAnimation.h"

BattleEntityPlayer *BattleEntityPlayer::create()
{
    BattleEntityPlayer *pPlayer = new BattleEntityPlayer();
    if (pPlayer && pPlayer->init())
    {
        pPlayer->autorelease();
        return pPlayer;
    }
    CC_SAFE_DELETE(pPlayer);
    return NULL;
}

BattleEntityPlayer::BattleEntityPlayer()
{

}
BattleEntityPlayer::~BattleEntityPlayer()
{

}

void BattleEntityPlayer::InitData()
{
	BattleEntity::InitData();
    SetEntityStatus(ES_RUN);
}

void BattleEntityPlayer::OnAnimationCreate(EntityAnimation* pNode)
{
	pNode->setPosition(ccp(getAnchorPointInPoints().x, getAnchorPointInPoints().y - getContentSize().height*0.5));
}

void BattleEntityPlayer::update(float dt)
{
	BattleEntity::update(dt);
}