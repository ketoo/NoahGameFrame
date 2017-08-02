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

    virtual int ExistSkill(const NFGUID& self, const std::string& strSkillName) = 0;

};

#endif