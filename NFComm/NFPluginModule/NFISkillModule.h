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


#ifndef NFI_SKILL_MODULE_H
#define NFI_SKILL_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFISkillModule
    : public NFIModule
{

public:
	virtual int UseSkill(const NFGUID& self, const std::string& strSkillName, const NFGUID& target, const int64_t index = 0) = 0;
	virtual int UseSkill(const NFGUID& self, const std::string& strSkillName, const NFDataList& target, const int64_t index = 0) = 0;

	/*
	std::cout << "100, -10: " << CalDamage(100, -10) << std::endl;
	std::cout << "100, 5: " << CalDamage(100, 5) << std::endl;
	std::cout << "100, 10: " << CalDamage(100, 10) << std::endl;
	std::cout << "100, 20: " << CalDamage(100, 20) << std::endl;
	std::cout << "100, 30: " << CalDamage(100, 30) << std::endl;
	std::cout << "-100, 30: " << CalDamage(-100, 30) << std::endl;
	std::cout << "-100, -30: " << CalDamage(-100, -30) << std::endl;

	 	100, -10: 250
		100, 5: 76
		100, 10: 62
		100, 20: 45
		100, 30: 35
		-100, 30: 0
		-100, -30: 0
	 */
	virtual int CalDamage(const int atk, const int def) = 0;

	//virtual int CalSkillDamage(const NFGUID& self, const int def) = 0;
};

#endif