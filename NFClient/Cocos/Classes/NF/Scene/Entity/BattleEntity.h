#ifndef __BattleEntity_H__
#define __BattleEntity_H__
class EntityAnimation;

enum EntityStatus
{
    ES_NONE,
    ES_IDLE,
    ES_IDLE2,
    ES_RUN,
    ES_WIN,
    ES_FALIURE,
    ES_LOOP_END,
    ES_ATTACK1,
    ES_ATTACK2,
    ES_SKILL1,
    ES_SKILL2,
    ES_HURT,   
    ES_DEAD,
    ES_MAX,
};

class BattleEntity : public Node
{
public:
    BattleEntity();
    ~BattleEntity();
    virtual bool init();
    virtual void InitData();
    virtual EntityAnimation* CreateAnimation();
    virtual void OnAnimationCreate(EntityAnimation* pNode);
    virtual void Play(const char *pAnimName, bool bLoop = true, const std::function<void()> &endFun = nullptr);
    virtual void update(float dt);

    virtual void UpdateState(float dt);
    virtual void OnStatusChange(EntityStatus oldStatus, EntityStatus newStatus);
    virtual void OnDead();
    void SetEntityStatus(EntityStatus status);
    EntityStatus GetEntityStatus();
    void SetPlayEnd(bool bPlayEnd);

    void SetAnimationRotation(float fAngle);
    Vec2 GetBoneWorldPosition(const char *pBoneName);
    void setEventListener(const std::function<void(const char *pName, float fTime)> &listener);
    Node *GetAnimationNode();

protected:
    EntityStatus m_CurStatus;
    EntityStatus m_Status;

protected:
    EntityAnimation *m_pAnimationNode;
	std::vector<std::string> m_vecAnimation;
};

#endif // __BattleEntity_H__
