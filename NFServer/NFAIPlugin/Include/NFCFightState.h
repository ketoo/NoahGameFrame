// -------------------------------------------------------------------------
//    文件名      ：    NFCFightState.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：   战斗状态
//
// -------------------------------------------------------------------------

#ifndef _NFC_FIGHTSTATE_H_
#define _NFC_FIGHTSTATE_H_

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

    virtual bool Enter(const NFIDENTID& self);

    virtual bool Execute(const NFIDENTID& self);

    virtual bool Exit(const NFIDENTID& self);

    virtual bool DoRule(const NFIDENTID& self);

protected:
    bool RunInFightArea(const NFIDENTID& self);
    bool RunCloseTarget(const NFIDENTID& self);

	int OnSkillConsumeTime(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& arg);

private:
};

#endif