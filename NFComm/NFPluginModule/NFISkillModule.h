// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_SKILL_MODULE_H
#define NFI_SKILL_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFISkillModule
    : public NFIModule
{

public:


    virtual int AddSkill(const NFGUID& self, const std::string& strSkillName) = 0;


    virtual int ExistSkill(const NFGUID& self, const std::string& strSkillName) = 0;


    virtual int SetSkillLevel(const NFGUID& self, const std::string& strSkillName, const int nLevel) = 0;


    virtual int SetSkillGem(const NFGUID& self, const std::string& strSkillName, const std::string& strGemName) = 0;


    virtual int SetSkillGemLevel(const NFGUID& self, const std::string& strSkillName, const int nLevel) = 0;


    virtual int GetSkillLevel(const NFGUID& self, const std::string& strSkillName) = 0;
    \


    virtual std::string GetSkillGem(const NFGUID& self, const std::string& strSkillName) = 0;


    virtual int GetSkillGemLevel(const NFGUID& self, const std::string& strSkillName) = 0;


    virtual int AddNewerSkill(const NFGUID& self) = 0;

    virtual int OnUseSkill(const NFGUID& self, const NFDataList& var) = 0;
};

#endif