// -------------------------------------------------------------------------
//    �ļ���      ��    NFCChaseState.h
//    ������      ��    eliteYang
//    ����ʱ��    ��    2014-12-04 17:51
//    ��������    ��    Ѳ��״̬
//
// -------------------------------------------------------------------------

#ifndef __NFC_CHASE_STATE_H_
#define __NFC_CHASE_STATE_H_

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