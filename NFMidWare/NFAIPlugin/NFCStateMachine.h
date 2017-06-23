// -------------------------------------------------------------------------
//    @FileName			:    NFCStateMachine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCStateMachine
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_STATE_MACHINE_H
#define NFC_STATE_MACHINE_H

#include "NFCDeadState.h"
#include "NFCFightState.h"
#include "NFCIdleState.h"
#include "NFCPatrolState.h"
#include "NFCChaseState.h"
#include "NFIStateMachine.h"
#include "NFCAIModule.h"

class NFCStateMachine
    : public NFIStateMachine
{
public:

    NFCStateMachine(const NFGUID& self, NFIPluginManager* p);

    virtual ~NFCStateMachine();

    virtual bool Execute();


	virtual NFIState* GetState(const NFAI_STATE eState) const;
	
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
    NFGUID mOwnerID;
    float mfHeartBeatTime;
    NFAI_STATE meCurrentState;
    NFAI_STATE meLastState;

protected:
	NFIAIModule* m_pAIModule;
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIMoveModule* m_pMoveModule;
	NFIElementModule* m_pElementModule;
};

#endif