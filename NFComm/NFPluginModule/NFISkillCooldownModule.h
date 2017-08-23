// -------------------------------------------------------------------------
//    @FileName         :   NFISkillCooldownModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-08-28
//    @Module           :   NFISkillCooldownModule

// -------------------------------------------------------------------------

#ifndef NFI_SKILL_COOLDOWN_MODULE_H
#define NFI_SKILL_COOLDOWN_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFISkillCooldownModule
    : public NFIModule
{
public:

    virtual void AddSkillCD( const NFGUID& self, const std::string& strSkillName ) = 0;
    virtual bool ExistSkillCD( const NFGUID& self, const std::string& strSkillName ) = 0;
};

#endif