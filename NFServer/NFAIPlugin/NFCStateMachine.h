// -------------------------------------------------------------------------
//    文件名      ：    NFIStateMachine.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：    状态机基类
//
// -------------------------------------------------------------------------

#ifndef _NFC_STATE_MACHINE_H_
#define _NFC_STATE_MACHINE_H_

#include "NFIStateMachine.h"
#include "NFCAIModule.h"

class NFCStateMachine
    : public NFIStateMachine
{
public:

    NFCStateMachine(const NFIDENTID& self, NFIAIModule* pControl);

    virtual ~NFCStateMachine();

    virtual void UpData(float fFrameTime, float fAllTime);

    //change to a new state
    virtual void ChangeState(const NFAI_STATE eState);

    virtual void RevertToLastState();

    virtual const NFAI_STATE CurrentState() const
    {
        return meCurrentState;
    }

    virtual const NFAI_STATE LastState() const
    {
        return meLastState;
    }


private:
    NFIAIModule*    m_pAIControlInterface;

    NFIDENTID mOwnerID;
    float mfHeartBeatTime;
    NFAI_STATE meCurrentState;
    NFAI_STATE meLastState;

protected:
    NFIKernelModule* m_pKernelModule;
};

#endif