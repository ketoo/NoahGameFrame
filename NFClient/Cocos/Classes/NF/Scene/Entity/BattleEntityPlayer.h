#ifndef __BattleEntityPlayer_H__
#define __BattleEntityPlayer_H__
#include "BattleEntity.h"

class BattleEntityPlayer : public BattleEntity
{
public:
    static BattleEntityPlayer *create();
    BattleEntityPlayer();
    ~BattleEntityPlayer();

    virtual void InitData();
    virtual void OnAnimationCreate(EntityAnimation* pNode);
    virtual void update(float dt);

private:
};

#endif // __BattleEntityPlayer_H__
