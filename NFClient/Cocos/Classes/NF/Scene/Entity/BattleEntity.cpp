#include "stdafx.h"

#include "BattleEntity.h"
#include "EntityAnimation.h"

BattleEntity::BattleEntity()
    : m_CurStatus(ES_NONE)
    , m_Status(ES_NONE)
{
}

BattleEntity::~BattleEntity()
{

}

bool BattleEntity::init()
{	
	m_vecAnimation.resize(ES_MAX);
	m_vecAnimation[ES_IDLE] = "Idle";
	m_vecAnimation[ES_IDLE2] = "Idle";
	m_vecAnimation[ES_RUN] = "Move";
	m_vecAnimation[ES_WIN] = "Cheer";
	m_vecAnimation[ES_FALIURE] = "failure";
	m_vecAnimation[ES_ATTACK1] = "atk";
	m_vecAnimation[ES_ATTACK2] = "atk";
	m_vecAnimation[ES_SKILL1] = "atk2";
	m_vecAnimation[ES_SKILL2] = "atk2";
	m_vecAnimation[ES_HURT] = "Damaged";
	m_vecAnimation[ES_DEAD] = "Death";

    InitData();
    m_pAnimationNode = CreateAnimation();
    addChild(m_pAnimationNode);
	m_pAnimationNode->setPosition(getAnchorPointInPoints());
	
	OnAnimationCreate(m_pAnimationNode);

    scheduleUpdate();
    return true;
}


void BattleEntity::InitData()
{
}

EntityAnimation* BattleEntity::CreateAnimation()
{
	EntityAnimation *pNode = NULL;
	pNode = EntityAnimationArmature::create();
	std::string strName = std::string("Armature/Pugna") + "/" + "Pugna" + ".ExportJson";
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(strName.c_str());
	CCArmature *pAnimation = CCArmature::create("Pugna");
	if (pAnimation)
		pNode->SetAnimationNode(pAnimation);

    if (!pNode)
    {
        pNode = EntityAnimation::create();
    }
    else
    {
        pNode->Play(m_vecAnimation[ES_IDLE].c_str());
    }

    return pNode;
}

void BattleEntity::OnAnimationCreate(EntityAnimation* pNode)
{
}

void BattleEntity::Play(const char *pAnimName, bool bLoop/* = true*/, const std::function<void()> &endFun/* = nullptr*/)
{
    if (m_pAnimationNode)
        m_pAnimationNode->Play(pAnimName, bLoop, endFun);
}

void BattleEntity::update(float dt)
{
    UpdateState(dt);
}

void BattleEntity::UpdateState(float dt)
{
    if (m_CurStatus != m_Status)
    {
        EntityStatus oldStatus = m_Status;
        m_CurStatus = m_Status;
        OnStatusChange(oldStatus, m_Status);
    }
}

void BattleEntity::SetEntityStatus(EntityStatus status)
{
    if (status == ES_HURT && m_Status != ES_IDLE)
        return;

    if (m_Status != ES_DEAD)
        m_Status = status;

    if (status > ES_LOOP_END)
    {
        m_CurStatus = ES_NONE;
        UpdateState(0.0f);
    }
}

EntityStatus BattleEntity::GetEntityStatus()
{
    return m_CurStatus;
}


void BattleEntity::SetAnimationRotation(float fAngle)
{
    m_pAnimationNode->setRotation(fAngle);
}
CCPoint BattleEntity::GetBoneWorldPosition(const char *pBoneName)
{
    return m_pAnimationNode->GetBoneWorldPosition(pBoneName);
}

void BattleEntity::setEventListener(const std::function<void(const char *pName, float fTime)> &listener)
{
    m_pAnimationNode->setEventListener(listener);
}

CCNode *BattleEntity::GetAnimationNode()
{
    return m_pAnimationNode;
}

void BattleEntity::OnStatusChange(EntityStatus oldStatus, EntityStatus newStatus)
{
	const char *pAnimName = m_vecAnimation[newStatus].c_str();
    switch (newStatus)
    {
        case ES_IDLE:
        {
            Play(pAnimName);
        }
        break;
        case ES_RUN:
        {
            Play(pAnimName);
        }
        break;
        case ES_ATTACK1:
        {
            Play(pAnimName, false, [this](){SetEntityStatus(ES_IDLE);});
        }
        break;
        case ES_ATTACK2:
        {
            Play(pAnimName, false, [this](){SetEntityStatus(ES_IDLE); });
        }
        break;
        case ES_SKILL1:
        {
            Play(pAnimName, false, [this](){SetEntityStatus(ES_IDLE); });
        }
        break;
        case ES_SKILL2:
        {
            Play(pAnimName, false, [this](){SetEntityStatus(ES_IDLE); });
        }
        break;
        case ES_HURT:
        {
            Play(pAnimName, false, [this](){SetEntityStatus(ES_IDLE); });
        }
        break;
        case ES_DEAD:
        {
            Play(pAnimName, false, [this](){ OnDead();});
        }
        break;
        case ES_WIN:
        {
            Play(pAnimName);
        }
        break;
        case ES_FALIURE:
        {
            Play(pAnimName);
        }
        break;
    }
}

void BattleEntity::OnDead()
{
    // ’‚±ﬂ”Œœ∑À¿Õˆ≤ª“∆≥˝
    //removeFromParentAndCleanup(true);
}
