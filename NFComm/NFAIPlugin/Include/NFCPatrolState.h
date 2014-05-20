// -------------------------------------------------------------------------
//    �ļ���      ��    NFCPatrolState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   Ѳ��״̬
//
// -------------------------------------------------------------------------

#ifndef _NFC_PATROL_H_
#define _NFC_PATROL_H_

#include "NFIState.h"

class NFCPatrolState
    : public NFIState
{
public:
    NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p)
        : NFIState(PatrolState, fHeartBeatTime, p)
    {
    }

    virtual ~NFCPatrolState() {}

    virtual bool Enter(const NFIDENTID& self);

    virtual bool Execute(const NFIDENTID& self);

    virtual bool Exit(const NFIDENTID& self);

    virtual bool DoRule(const NFIDENTID& self);

protected:

    bool RandomPatrol(const NFIDENTID& self);

private:
};

#endif