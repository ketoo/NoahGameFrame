// -------------------------------------------------------------------------
//    @FileName			:    NFIState.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFIState
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFI_STATE_H
#define NFI_STATE_H

#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

//所有的状态
enum NFAI_NPC_TYPE
{
	BUILDING_TYPE,
	MASTER_TYPE,
	HERO_TYPE,
};

//所有的状态
enum NFAI_STATE
{
    State_Error = 0,
    IdleState,                  // 休闲状态
    PatrolState,                // 巡逻状态
    FightState,                 // 战斗状态
    ChaseState,                 // 追击状态
    DeadState,                  // 死亡状态
};

class NFIStateMachine;

class NFIState
	: public NFIModule
{
public:
    NFIState(NFAI_STATE eState, const float fHeartBeatTime, NFIPluginManager* p)
    {
        meState = eState;
        mfHeartBeatTime = fHeartBeatTime;
        pPluginManager = p;
    }

    virtual ~NFIState()
    {
    }

    virtual bool DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        //返回true表示基类处理过，上面就别处理了
        return false;
    }

    virtual bool Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    virtual bool Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    virtual bool Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    const NFAI_STATE GetCurrState()
    {
        return meState;
    }

    float GetHeartBeatTime()
    {
        return mfHeartBeatTime;
    }

protected:
    NFAI_STATE  meState;
    float               mfHeartBeatTime;
};

#endif