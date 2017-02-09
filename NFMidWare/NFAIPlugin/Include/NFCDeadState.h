// -------------------------------------------------------------------------
//    @FileName			:    NFCDeadState.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCDeadState
//    @Desc             :
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