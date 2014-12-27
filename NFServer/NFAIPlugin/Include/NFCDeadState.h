// -------------------------------------------------------------------------
//    �ļ���      ��    NFCDeadState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   ����״̬
//
// -------------------------------------------------------------------------

#ifndef _NFC_DEADSTATE_H_
#define _NFC_DEADSTATE_H_

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

    virtual bool Enter(const NFIDENTID& self);

    virtual bool Execute(const NFIDENTID& self);

    virtual bool Exit(const NFIDENTID& self);

    virtual bool DoRule(const NFIDENTID& self);

protected:
private:
};

#endif