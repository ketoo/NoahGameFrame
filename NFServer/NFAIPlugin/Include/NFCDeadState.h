// -------------------------------------------------------------------------
//    文件名      ：    NFCDeadState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   死亡状态
//
// -------------------------------------------------------------------------

#ifndef NFC_DEADSTATE_H
#define NFC_DEADSTATE_H

#include "NFIState.h"

class NFCDeadState
    : public NFIState
{
public:
    NFCDeadState(float fHeartBeatTime, NFIPluginManager* p)
        : NFIState(DeadState, fHeartBeatTime, p)
    {
    }

    virtual ~NFCDeadState() {}

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:
private:
};

#endif