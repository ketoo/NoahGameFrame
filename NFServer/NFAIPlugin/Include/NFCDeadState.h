// -------------------------------------------------------------------------
//    �ļ���      ��    NFCDeadState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   ����״̬
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