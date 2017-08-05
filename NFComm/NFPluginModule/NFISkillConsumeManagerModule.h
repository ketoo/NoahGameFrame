// -------------------------------------------------------------------------
//    @FileName      :   NFISkillConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeManagerModule

// -------------------------------------------------------------------------

#ifndef NFI_SKILLCONSUME_MANAGER_MODULE_H
#define NFI_SKILLCONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFISkillConsumeProcessModule.h"

enum SKILL_CONSUME_TYPE
{
	SKILL_BRIEF,
	SKILL_BULLET,
};

class NFISkillConsumeManagerModule
    : public NFIModule
{
public:
    virtual bool ResgisterConsumeModule(const int nModuleType, NF_SHARE_PTR<NFISkillConsumeProcessModule> pModule) = 0;

    virtual NF_SHARE_PTR<NFISkillConsumeProcessModule> GetConsumeModule(const int nModuleType) = 0;

};

#endif