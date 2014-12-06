// -------------------------------------------------------------------------
//    文件名      ：    NFCChaseState.h
//    创建者      ：    eliteYang
//    创建时间    ：    2014-12-04 17:51
//    功能描述    ：    巡逻状态
//
// -------------------------------------------------------------------------

#ifndef __NFC_CHASE_STATE_H__
#define __NFC_CHASE_STATE_H__

#include "NFIState.h"

class NFCChaseState : public NFIState
{
public:
    NFCChaseState(float fHeartBeatTime, NFIPluginManager* p)
        : NFIState(ChaseState, fHeartBeatTime, p)
    {
    }

    virtual ~NFCChaseState() {}

    virtual bool Enter(const NFIDENTID& self);

    virtual bool Execute(const NFIDENTID& self);

    virtual bool Exit(const NFIDENTID& self);

    virtual bool DoRule(const NFIDENTID& self);

protected:

    //bool RandomPatrol(const NFIDENTID& self);

private:
};

#endif // !__NFC_CHASE_STATE_H__