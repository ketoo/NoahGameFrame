// -------------------------------------------------------------------------
//    文件名      ：    NFIState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   状态类
//
// -------------------------------------------------------------------------

#ifndef _NFI_STATE_H_
#define _NFI_STATE_H_

#include "NFComm/NFCore/NFIDataList.h"

class NFIMoveModule;
class NFIAIModule;
class NFIHateModule;
class NFIKernelModule;
class NFIEventProcessModule;
class NFIElementInfoModule;
class NFIPluginManager;

//所有的状态
enum NFAI_STATE
{
    State_Error = 0,
    IdleState,                   //休闲状态
    PatrolState,                //巡逻状态
    FightState,                 //战斗状态
    DeadState,                  //死亡状态
};

class NFIState
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

    virtual bool DoRule(const NFIDENTID& self)
    {
        //返回true表示基类处理过，上面就别处理了
        return false;
    }

    virtual bool Enter(const NFIDENTID& self)
    {
        return false;
    }

    virtual bool Execute(const NFIDENTID& self)
    {
        return false;
    }

    virtual bool Exit(const NFIDENTID& self)
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

    virtual bool DoEnterScript(const NFIDENTID& self)
    {
        return true;
    }

    virtual bool DoExecuteScript(const NFIDENTID& self)
    {
        return true;
    }

    virtual bool DoExitScript(const NFIDENTID& self)
    {
        return true;
    }

protected:
    NFAI_STATE  meState;
    float               mfHeartBeatTime;
    NFIPluginManager* pPluginManager;

    static NFIAIModule* m_pAIModule;
    static NFIHateModule* m_pHateModule;
    static NFIKernelModule* m_pKernelModule;
    static NFIMoveModule* m_pMoveModule;
    static NFIElementInfoModule* m_pElementInfoModule;
    static NFIEventProcessModule* m_pEventProcessModule;

};

#endif