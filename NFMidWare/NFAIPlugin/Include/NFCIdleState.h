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

class NFIStateMachine;

class NFCIdleState
    : public NFIState
{
public:
    NFCIdleState(float fHeartBeatTime, NFIPluginManager* p);
    virtual ~NFCIdleState();

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:
    bool RandomIdle(const NFGUID& self, NFIStateMachine* pStateMachine);
private:
};

#endif