// -------------------------------------------------------------------------
//    @FileName			:    NFCStateMachine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCStateMachine
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_STATE_MACHINE_H
#define NFC_STATE_MACHINE_H

#include "NFIStateMachine.h"
#include "NFCAIModule.h"

class NFCStateMachine
    : public NFIStateMachine
{
public:

    NFCStateMachine(const NFGUID& self, NFIAIModule* pControl);

    virtual ~NFCStateMachine();

    virtual void Execute();

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

    NFGUID mOwnerID;
    float mfHeartBeatTime;
    NFAI_STATE meCurrentState;
    NFAI_STATE meLastState;

protected:
    NFIKernelModule* m_pKernelModule;
};

#endif