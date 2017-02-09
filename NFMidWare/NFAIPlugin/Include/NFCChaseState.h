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

class NFCChaseState : public NFIState
{
public:
    NFCChaseState(float fHeartBeatTime, NFIPluginManager* p)
        : NFIState(ChaseState, fHeartBeatTime, p)
    {
    }

    virtual ~NFCChaseState() {}

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:

    //bool RandomPatrol(const NFGUID& self);

private:
};

#endif // !__NFC_CHASE_STATE_H_