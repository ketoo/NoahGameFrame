// -------------------------------------------------------------------------
//    @FileName			:    NFCFightState.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCFightState
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_FIGHTSTATE_H
#define NFC_FIGHTSTATE_H

#include "NFIState.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"

class NFCFightState
    : public NFIState
{
public:
	NFCFightState(float fHeartBeatTime, NFIPluginManager* p);

    virtual ~NFCFightState() {}

    virtual bool Enter(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Execute(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool Exit(const NFGUID& self, NFIStateMachine* pStateMachine);

    virtual bool DoRule(const NFGUID& self, NFIStateMachine* pStateMachine);

protected:
    bool RunInFightArea(const NFGUID& self, NFIStateMachine* pStateMachine);
    bool RunCloseTarget(const NFGUID& self, const NFGUID& target, NFIStateMachine* pStateMachine);

	int OnSkillConsumeTime(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

private:
	NFIAIModule* m_pAIModule;
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIMoveModule* m_pMoveModule;
	NFIElementModule* m_pElementModule;
};

#endif