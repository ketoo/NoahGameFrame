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


#ifndef NFI_SKILL_CONSUME_MANAGER_MODULE_H
#define NFI_SKILL_CONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFISkillConsumeProcessModule
		: public NFIModule
{
public:

	virtual int ConsumeLegal(const NFGUID& self, const std::string& skillID,  const NFDataList& other) = 0;
	virtual int ConsumeProcess(const NFGUID& self, const std::string& strSkillName, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList) = 0;
};

class NFISkillConsumeManagerModule
    : public NFIModule
{
public:
	virtual bool SetConsumeModule(const int skillType, NFISkillConsumeProcessModule* pModule) = 0;
	virtual bool SetConsumeModule(const int skillType, const int skillSubType, NFISkillConsumeProcessModule* pModule) = 0;

	virtual NFISkillConsumeProcessModule* GetConsumeModule(const int skillType) = 0;
	virtual NFISkillConsumeProcessModule* GetConsumeModule(const int skillType, const int skillSubType) = 0;

};

#endif