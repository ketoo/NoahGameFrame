// -------------------------------------------------------------------------
//    文件名      ：    NFCPatrolState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   巡逻状态
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