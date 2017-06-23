// -------------------------------------------------------------------------
//    @FileName			:    NFCChaseState.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCChaseState
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_CHASE_STATE_H_
#define NFC_CHASE_STATE_H_

#include "NFIState.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"

class NFCChaseState : public NFIState
{
public:
	NFCChaseState(float fHeartBeatTime, NFIPluginManager* p);

    virtual ~NFCChaseState() {}

    virtual bool Enter(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Execute(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Exit(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool DoRule(const NFGUID& self, NFIStateMachine* pStateMachine);

protected:

    //bool RandomPatrol(const NFGUID& self);

private:
	NFIAIModule* m_pAIModule;
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIMoveModule* m_pMoveModule;
	NFIElementModule* m_pElementModule;
};

#endif // !__NFC_CHASE_STATE_H_