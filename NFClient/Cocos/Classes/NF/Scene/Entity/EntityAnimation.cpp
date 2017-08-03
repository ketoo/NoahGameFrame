#include "stdafx.h"

#include "EntityAnimation.h"

EntityAnimation::EntityAnimation()
    : m_pAnimationNode(NULL)
{
}

EntityAnimation::~EntityAnimation()
{
    m_pAnimationNode = NULL;
}

void EntityAnimation::Play(const char *pAnimName, bool bLoop/* = true*/, const std::function<void()> &endFun/* = nullptr*/)
{
}

void EntityAnimation::OnAnimationEnd()
{
    if (m_endFun)
        m_endFun();

	m_endFun = nullptr;
}

void EntityAnimation::SetAnimationNode(CCNode *pNode)
{
    if (m_pAnimationNode)
    {
        m_pAnimationNode->removeFromParentAndCleanup(true);
    }
    m_pAnimationNode = pNode;
    addChild(m_pAnimationNode);
}


void EntityAnimationArmature::Play(const char *pAnimName, bool bLoop/* = true*/, const std::function<void()> &endFun/* = nullptr*/)
{
    if (m_pAnimationNode)
    {
        CCArmature* armatureAnimation = (CCArmature*)m_pAnimationNode;
        armatureAnimation->getAnimation()->play(pAnimName, -1, -1, bLoop ? -1 : 0);

		m_endFun = endFun;

        if (endFun != nullptr)
        {
            armatureAnimation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(EntityAnimationArmature::OnMovementEvent));
        }
        else
        {
			armatureAnimation->getAnimation()->setMovementEventCallFunc(NULL, NULL);
        }
    }
}

void EntityAnimationArmature::OnMovementEvent(CCArmature *, MovementEventType, const char *name)
{
	CCArmature* armatureAnimation = (CCArmature*)m_pAnimationNode;
	if(m_endFun)
	{
		m_endFun();
		m_endFun = nullptr;
		armatureAnimation->getAnimation()->setMovementEventCallFunc(NULL, NULL);
	}
}
