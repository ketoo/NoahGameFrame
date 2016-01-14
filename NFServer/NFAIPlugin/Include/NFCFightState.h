// -------------------------------------------------------------------------
//    文件名      ：    NFCFightState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   战斗状态
//
// -------------------------------------------------------------------------

#ifndef NFC_FIGHTSTATE_H
#define NFC_FIGHTSTATE_H

#include "NFIState.h"

class NFCFightState
    : public NFIState
{
public:
    NFCFightState(float fHeartBeatTime, NFIPluginManager* p)
        : NFIState(FightState, fHeartBeatTime, p)
    {
    }

    virtual ~NFCFightState() {}

    virtual bool Enter(const NFGUID& self);

    virtual bool Execute(const NFGUID& self);

    virtual bool Exit(const NFGUID& self);

    virtual bool DoRule(const NFGUID& self);

protected:
    bool RunInFightArea(const NFGUID& self);
    bool RunCloseTarget(const NFGUID& self, const NFGUID& target);

	int OnSkillConsumeTime(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

private:
};

#endif