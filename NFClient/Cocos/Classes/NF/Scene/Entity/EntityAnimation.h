#ifndef __EntityAnimation_H__
#define __EntityAnimation_H__

class EntityAnimation : public CCNode
{
public:
    EntityAnimation();
    ~EntityAnimation();
    CREATE_FUNC(EntityAnimation);
    virtual void Play(const char *pAnimName, bool bLoop = true, const std::function<void()> &endFun = nullptr);
    virtual void ChangeSkin(const char *pSkineName) {};
    void SetAnimationNode(CCNode *pNode);
    virtual CCPoint GetBoneWorldPosition(const char *pBoneName) { return convertToWorldSpace(CCPointZero); };
    virtual void setEventListener(const std::function<void(const char *pName, float fTime)> &listener) {};

protected:
	void OnAnimationEnd();

protected:
    CCNode *m_pAnimationNode;
	std::function<void()> m_endFun;
};

class EntityAnimationArmature : public EntityAnimation
{
public:
    CREATE_FUNC(EntityAnimationArmature);
    virtual void Play(const char *pAnimName, bool bLoop = true, const std::function<void()> &endFun = nullptr);
    virtual void ChangeSkin(const char *pSkineName) {};

	void OnMovementEvent(CCArmature *, MovementEventType, const char *name);
};
#endif // __EntityAnimation_H__
