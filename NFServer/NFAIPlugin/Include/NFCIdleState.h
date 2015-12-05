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

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:
    bool RandomIdle(const NFGUID& self, NFIStateMachine* pStateMachine);
private:
};

#endif