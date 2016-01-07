// -------------------------------------------------------------------------
//    文件名      ：    NFIStateMachine.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：    状态机基类
//
// -------------------------------------------------------------------------

#ifndef _NFI_STATE_MACHINE_H
#define _NFI_STATE_MACHINE_H

#include "Include/NFCDeadState.h"
#include "Include/NFCFightState.h"
#include "Include/NFCIdleState.h"
#include "Include/NFCPatrolState.h"
#include "Include/NFCChaseState.h"

class NFIStateMachine
{
public:

    virtual const NFAI_STATE  CurrentState() const = 0;
    virtual const NFAI_STATE  LastState() const = 0;

    //call this to update the FSM
    virtual void Execute() = 0;

    //change to a new state
    virtual void ChangeState(const NFAI_STATE eState) = 0;

    //return last state
    virtual void RevertToLastState() = 0;

protected:

private:
};

#endif