// -------------------------------------------------------------------------
//    @FileName			:    NFCIdleState.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCIdleState
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_IDLESTATE_H
#define NFC_IDLESTATE_H

#include "NFIState.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"

class NFCIdleState
    : public NFIState
{
public:
	NFCIdleState(float fHeartBeatTime, NFIPluginManager* p);

    virtual ~NFCIdleState();

    virtual bool Enter(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Execute(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Exit(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool DoRule(const NFGUID& self, NFIStateMachine* pStateMachine);

protected:
    bool RandomIdle(const NFGUID& self, NFIStateMachine* pStateMachine);
private:
	NFIAIModule* m_pAIModule;
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIMoveModule* m_pMoveModule;
	NFIElementModule* m_pElementModule;
};

#endif