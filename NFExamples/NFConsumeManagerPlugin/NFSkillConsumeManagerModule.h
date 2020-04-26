/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef NF_SKILL_CONSUME_MANAGER_MODULE_H
#define NF_SKILL_CONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"

class NFSkillConsumeManagerModule
    : public NFISkillConsumeManagerModule
{
public:
    NFSkillConsumeManagerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init() override;
    virtual bool Shut() override;
    virtual bool Execute() override;
    virtual bool AfterInit() override;

    virtual bool SetConsumeModule(const int skillType, NFISkillConsumeProcessModule* pModule);
	virtual bool SetConsumeModule(const int skillType, const int skillSubType, NFISkillConsumeProcessModule* pModule);

	virtual NFISkillConsumeProcessModule* GetConsumeModule( const int skillType);
	virtual NFISkillConsumeProcessModule* GetConsumeModule( const int skillType, const int skillSubType);

private:
	std::map<NFGUID, NFISkillConsumeProcessModule*> mSkillConsumeProcess;
};

#endif
