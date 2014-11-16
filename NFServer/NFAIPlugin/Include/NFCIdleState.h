// -------------------------------------------------------------------------
//    文件名      ：    NFCIdleState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   休闲状态
//
// -------------------------------------------------------------------------

#ifndef _NFC_IDLESTATE_H_
#define _NFC_IDLESTATE_H_

#include "NFIState.h"

class NFIStateMachine;

class NFCIdleState
    : public NFIState
{
public:
    NFCIdleState(float fHeartBeatTime, NFIPluginManager* p);
    virtual ~NFCIdleState();

    virtual bool Enter(const NFIDENTID& self);

    virtual bool Execute(const NFIDENTID& self);

    virtual bool Exit(const NFIDENTID& self);

    virtual bool DoRule(const NFIDENTID& self);

protected:
    bool RandomIdle(NFIStateMachine* pStateMachine);
private:
};

#endif