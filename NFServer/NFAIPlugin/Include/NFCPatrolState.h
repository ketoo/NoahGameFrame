// -------------------------------------------------------------------------
//    �ļ���      ��    NFCPatrolState.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-05 22:11
//    ��������    ��   Ѳ��״̬
//
// -------------------------------------------------------------------------

#ifndef NFC_PATROL_H
#define NFC_PATROL_H

#include "NFIState.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCPatrolState
    : public NFIState
{
public:
    NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p);

    virtual ~NFCPatrolState() {}

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:

    bool RandomPatrol(const NFGUID& self);

private:
};

#endif