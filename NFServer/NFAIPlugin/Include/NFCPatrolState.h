// -------------------------------------------------------------------------
//    文件名      ：    NFCPatrolState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   巡逻状态
//
// -------------------------------------------------------------------------

#ifndef _NFC_PATROL_H_
#define _NFC_PATROL_H_

#include "NFIState.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCPatrolState
    : public NFIState
{
public:
    NFCPatrolState(float fHeartBeatTime, NFIPluginManager* p);

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