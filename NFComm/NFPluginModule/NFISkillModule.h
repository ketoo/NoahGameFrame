// -------------------------------------------------------------------------
//    @FileName         ：    NFISkillModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFISkillModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SKILL_MODULE_H_
#define _NFI_SKILL_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFISkillModule
    : public NFILogicModule
{

public:

    //添加技能
    virtual int AddSkill(const NFIDENTID& self, const std::string& strSkillName) = 0;

    //当前是否存在技能
    virtual int ExistSkill(const NFIDENTID& self, const std::string& strSkillName) = 0;

    //设置技能等级
    virtual int SetSkillLevel(const NFIDENTID& self, const std::string& strSkillName, const int nLevel) = 0;

    //设置技能宝石
    virtual int SetSkillGem(const NFIDENTID& self, const std::string& strSkillName, const std::string& strGemName) = 0;

    //设置技能宝石等级
    virtual int SetSkillGemLevel(const NFIDENTID& self, const std::string& strSkillName, const int nLevel) = 0;

    //得到技能等级
    virtual int GetSkillLevel(const NFIDENTID& self, const std::string& strSkillName) = 0;
    \

    //得到技能宝石
    virtual std::string GetSkillGem(const NFIDENTID& self, const std::string& strSkillName) = 0;

    //得到技能宝石等级
    virtual int GetSkillGemLevel(const NFIDENTID& self, const std::string& strSkillName) = 0;

    //添加新手技能
    virtual int AddNewerSkill(const NFIDENTID& self) = 0;

    virtual int OnUseSkill(const NFIDENTID& self, const NFIDataList& var) = 0;
};

#endif